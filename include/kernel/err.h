/**
 * /include/kernel/err.h
 *
 * Copyright (C) 2017 HinsShum
 *
 * err.h is free software: you can redistribute it and/or modify
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

#ifndef _KERNEL_ERR_H
#define _KERNEL_ERR_H

/*---------- includes ----------*/
#include <kernel/compiler.h>

/*---------- marcos ----------*/
/**
 * Kernel pointers have redundant information, so we can use a
 * scheme where we can return either an error code or a dentry
 * pointer with the same return value.
 *
 * This should be a per-architecture thing, to allow different
 * error and pointer decisions.
 */
#define MAX_ERRNO       (4095)

#define IS_ERR_VALUE(x) unlikely((x) >= (unsigned long)-MAX_ERRNO)

/*---------- functions ----------*/
static inline void * __must_check ERR_PTR(long error)
{
    return (void *)error;
}

static inline long __must_check PTR_ERR(const void *ptr)
{
    return (long)ptr;
}

static inline long __must_check IS_ERR(const void *ptr)
{
    return IS_ERR_VALUE((unsigned long)ptr);
}

static inline long __must_check IS_ERR_OR_NULL(const void *ptr)
{
    return !ptr || IS_ERR_VALUE((unsigned long)ptr);
}

/**
 * ERR_CAST - Explicitly cast an error-valued pointer to another pointer type
 * @ptr: The pointer to cast.
 *
 * Explicitly cast an error-valued pointer to another pointer type in such a
 * way as to make it clear that's what's going on.
 */
static inline void * __must_check ERR_CAST(const void *ptr)
{
    /* cast away the const */
    return (void *) ptr;
}

#endif /* _KERNEL_ERR_H */
