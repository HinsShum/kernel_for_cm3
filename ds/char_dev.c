/**
 * /kernel/char_dev.c
 *
 * Copyright (C) 2017 HinsShum
 *
 * char_dev.c is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

/*---------- includes ----------*/
#include <kernel/ds.h>
#include <kernel/mem.h>
#include <kernel/err.h>
#include <kernel/mutex.h>
#include <kernel/kernel.h>
#include <kernel/kdev_t.h>

/*---------- data defines ----------*/
static DECLARE_MUTEX(chrdevs_lock);

static struct char_device_struct {
    struct char_device_struct *next;
    unsigned int major;
    unsigned int baseminor;
    int minorct;
    char name[32];
    //struct cdev *cdev;
} *chrdevs[CHRDEV_MAJOR_HASH_SIZE];

/*----------functions ----------*/

/**
 * index in the above - for now: assume no multimajor ranges.
 */
static inline int major_to_index(int major)
{
    return major % CHRDEV_MAJOR_HASH_SIZE;
}

/**
 * __register_chrdev_region() - Register a single major with a
 * specified minor range.
 *
 * If major == 0 this functions will dynamically allocate a
 * major and return its number.
 *
 * If major > 0 this function will attempt to reserve the passed
 * range of minors and will return zero on success.
 *
 * Returns: a -ve error number on failure.
 */
static struct char_device_struct *__register_chrdev_region(unsigned int major, unsigned int baseminor,
                                                           int minorct, const char *name)
{
    struct char_device_struct *cd = NULL, **cp = NULL;
    int i = 0, ret = 0;

    cd = kzalloc(sizeof(struct char_device_struct));
    if(cd == NULL) {
        //return ERR_PTR(-ENOMEM);
        return ERR_PTR(-12);
    }

    mutex_lock_blocked(&chrdevs_lock);

    if(major == 0) {
        for(i = ARRAY_SIZE(chrdevs) - 1; i > 0; i--) {
            if(chrdevs[i] == NULL) {
                break;
            }
        }
        if(i == 0) {
            //ret = -EBUSY;
            ret = -16;
            goto out;
        }
        major = i;
    }
    cd->major = major;
    cd->baseminor = baseminor;
    cd->minorct = minorct;
    strlcpy(cd->name, name, sizeof(cd->name));

    i = major_to_index(major);

    for(cp = &chrdevs[i]; *cp; cp = &(*cp)->next) {
        if((*cp)->major > major ||
           ((*cp)->major == major &&
            (((*cp)->baseminor >= baseminor) ||
             ((*cp)->baseminor + (*cp)->minorct > baseminor)))) {
            break;
        }
    }
    /* Check for overlapping minor ranges. */
    if(*cp && (*cp)->major == major) {
        int old_min = (*cp)->baseminor;
        int old_max = (*cp)->baseminor + (*cp)->minorct - 1;
        int new_min = baseminor;
        int new_max = baseminor + minorct - 1;

        /* New driver overlaps from the right. */
        if(new_min >= old_min && new_min <= old_max) {
            //ret = -EBUSY;
            ret = -16;
            goto out;
        }
        /* New driver overlaps from the left. */
        if(new_max >= old_min && new_max <= old_max) {
            //ret = -EBUSY;
            ret = -16;
            goto out;
        }
    }

    cd->next = *cp;
    *cp = cd;
    mutex_unlock(&chrdevs_lock);
    return cd;
out:
    mutex_unlock(&chrdevs_lock);
    kfree(cd);
    return ERR_PTR(ret);
}

static struct char_device_struct *__unregister_chrdev_region(unsigned int major, unsigned int baseminor,
                                                             int minorct)
{
    struct char_device_struct *cd = NULL, **cp = NULL;
    int i = major_to_index(major);

    mutex_lock_blocked(&chrdevs_lock);
    for(*cp = chrdevs[i]; *cp; cp = &(*cp)->next) {
        if((*cp)->major == major &&
           (*cp)->baseminor == baseminor &&
           (*cp)->minorct == minorct) {
            break;
        }
    }
    if(*cp) {
        cd = *cp;
        *cp = cd->next;
    }
    mutex_unlock(&chrdevs_lock);
    return cd;
}

/**
 * register_chrdev_region() - register a range of device
 * numbers.
 *
 * @param from: the first in the desired range of device
 *            numbers; must include the major number.
 * @param count: the number of consecutive device numbers
 *             required.
 * @param name: the name of the device or driver.
 *
 * Returns: Return value is zero on success, a negative error
 * code on failure.
 */
int register_chrdev_region(dev_t from, unsigned int count, const char *name)
{
    struct char_device_struct *cd = NULL;
    dev_t to = from + count;
    dev_t n = 0, next = 0;

    for(n = from; n < to; n = next) {
        next = MKDEV(MAJOR(n) + 1, 0);
        if(next > to) {
            next = to;
        }
        cd = __register_chrdev_region(MAJOR(n), MINOR(n), next - n, name);
        if(IS_ERR(cd)) {
            goto fail;
        }
    }
    return 0;
fail:
    to = n;
    for(n = from; n < to; n = next) {
        next = MKDEV(MAJOR(n) + 1, 0);
        kfree(__unregister_chrdev_region(MAJOR(n), MINOR(n), next - n));
    }
    return PTR_ERR(cd);
}

/**
 * alloc_chrdev_region() - Allocates a range of char device
 * numbers.  The major number will be chosen dynamically, and
 * returned (along with the first minor number) in @dev.
 *
 * @param dev: output parameter for first assigned number.
 * @param baseminor: first of the requested range of minor
 *                 numbers.
 * @param count: the number of minor numbers required.
 * @param name: the name of the associated device or driver.
 *
 * Returns: Returns zero or a negative error code.
 */
int alloc_chrdev_region(dev_t *dev, unsigned int baseminor,
                        unsigned int count, const char *name)
{
    struct char_device_struct *cd = NULL;
    cd = __register_chrdev_region(0, baseminor, count, name);
    if(IS_ERR(cd)) {
        return PTR_ERR(cd);
    }
    *dev = MKDEV(cd->major, cd->baseminor);
    return 0;
}

/**
 * unregister_chrdev_region() - return a range of device
 * numbers.
 *
 * @param from: the first in the range of numbers to unregister.
 * @param count: the number of device numbers to unregister.
 *
 * This function will unregister a range of @count device
 * numbers, starting with @from. The caller should normally be
 * the one who allocated those numbers in the first place...
 */
void unregister_chrdev_region(dev_t from, unsigned int count)
{
    dev_t to = from + count;
    dev_t n = 0, next = 0;

    for(n = from; n < to; n = next) {
        next = MKDEV(MAJOR(n) + 1, 0);
        if(next > to) {
            next = to;
        }
        kfree(__unregister_chrdev_region(MAJOR(n), MINOR(n), next - n));
    }
}

/**
 * __register_chrdev() - create and register a cdev occupying a
 * range of minors
 *
 * @param major: major device number or 0 for dynamic allocation
 * @param baseminor: first of the requested range of minor
 *                 numbers
 * @param count: the number of minor numbers required
 * @param name: name of this range of devices
 * @param dops: device operations associated with this devices
 *
 * If @major == 0 this functions will dynamically allocate a
 * major and return its number.
 *
 * If @major > 0 this function will attempt to reserve a device
 * with the given major number and will return zero on success.
 *
 * Returns: Register success will return major when
 * @param(major) is zero, otherwise will return zero.
 * Register failed will return error number.
 */
int __register_chrdev(unsigned int major, unsigned int baseminor,
                      unsigned int count, const char *name,
                      const struct dev_operations *dops)
{
    struct char_device_struct *cd = NULL;

    cd = __register_chrdev_region(major, baseminor, count, name);
    if(IS_ERR(cd)) {
        return PTR_ERR(cd);
    }
    /**
     * 要完成的事情: 关于cdev部分的接口注册设备．
     */

    return major ? 0 : cd->major;
}

/**
 * __unregister_chrdev() - unregister and destroy a cdev.
 *
 * @param major: major device number.
 * @param baseminor: first of the range of minor numbers.
 * @param count: the number of minor numbers this cdev is
 *             occupying.
 * @param name: name of this range of devices.
 *
 * Unregister the region described by @major, @baseminor and
 * @count.  This function undoes what __register_chrdev() did.
 */
void __unregister_chrdev(unsigned int major, unsigned int baseminor,
                         unsigned int count, const char *name)
{
    struct char_device_struct *cd = NULL;
    cd = __unregister_chrdev_region(major, baseminor, count);
    /**
     * 要完成的事情: 关于cdev部分的代码还没有理解,
     * 等理解后再补充这部分的代码.
     */
    kfree(cd);
}


