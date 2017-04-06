/**
 * /include/kernel/mem.h
 *
 * Copyright (C) 2017 HinsShum
 *
 * mem.h is free software: you can redistribute it and/or modify
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

#ifndef _KERNEL_MEM_H
#define _KERNEL_MEM_H

/*---------- includes ----------*/
#include <kernel/types.h>
#include <freertos/FreeRTOS.h>
#include <freertos/portable.h>
#include <string.h>

/*---------- functions ----------*/

/**
 * kmalloc() - allocate memory.
 *
 * @param size: how many bytes of memory are required.
 */
static inline void *kmalloc(_size_t size)
{
    return pvPortMalloc(size);
}

/**
 * kmalloc() - allocate memory. The memory is set to zero.
 *
 * @param size: how many bytes of memory are required.
 */
static inline void *kzalloc(_size_t size)
{
    void *ret;

    ret = kmalloc(size);
    if(ret != NULL) {
        memset((char *)ret, 0, size);
    }

    return ret;
}

/**
 * kfree() - free previously allocated memory.
 *
 * @param fr: pointer returned by kmalloc.
 */
static inline void kfree(void *fr)
{
    vPortFree(fr);
}

#endif /* _KERNEL_MEM_H */
