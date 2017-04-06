/**
 * /include/kernel/ds.h
 *
 * Copyright (C) 2017 HinsShum
 *
 * ds.h is free software: you can redistribute it and/or modify
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

#ifndef _KERNEL_DS_H
#define _KERNEL_DS_H

/*---------- includes ----------*/
#include <kernel/types.h>
#include <kernel/module.h>

/*---------- marcos ----------*/
#define CHRDEV_MAJOR_HASH_SIZE   (255)

/*---------- data types ----------*/
struct dev_operations {
    int (*write)(dev_t dev, const char *buf, _size_t count);
    int (*read)(dev_t dev, const char *buf, _size_t count);
    dev_t (*open)(const char *name);
};

/*---------- functions ----------*/
extern int alloc_chrdev_region(dev_t *dev, unsigned int baseminor,
                               unsigned int count, const char *name);
extern int register_chrdev_region(dev_t from, unsigned int count, const char *name);
extern void unregister_chrdev_region(dev_t from, unsigned int count);
extern int __register_chrdev(unsigned int major, unsigned int baseminor,
                             unsigned int count, const char *name,
                             const struct dev_operations *dops);
extern void __unregister_chrdev(unsigned int major, unsigned int baseminor,
                                unsigned int count, const char *name);

static inline int register_chrdev(unsigned int major, const char *name,
                                  const struct dev_operations *dops)
{
    return __register_chrdev(major, 0, 256, name, dops);
}

static inline void unregister_chrdev(unsigned int major, const char *name)
{
    __unregister_chrdev(major, 0, 256, name);
}

#endif /* _KERNEL_DS_H */
