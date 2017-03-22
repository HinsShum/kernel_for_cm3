/**
 * /include/kernel/kernel.h
 *
 * Copyright (C) 2017 HinsShum
 *
 * kernel.h is free software: you can redistribute it and/or modify
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

#ifndef __KERNEL_H
#define __KERNEL_H

/*---------- includes ----------*/
#include <stdarg.h>
#include <stddef.h>
#include <stdio.h>
#include <stdbool.h>
#include <kernel/types.h>

/*---------- marcos ----------*/
/**
 * log level
 */
#define KERN_EMERG      "<0>"   /* system is unusable */
#define KERN_ALERT      "<1>"   /* action must be taken immediately */
#define KERN_CRIT       "<2>"   /* critical conditions */
#define KERN_ERR        "<3>"   /* error conditions */
#define KERN_WARNING    "<4>"   /* warning conditions */
#define KERN_NOTICE     "<5>"   /* normal but significant condition */
#define KERN_INFO       "<6>"   /* informational */
#define KERN_DEBUG      "<7>"   /* debug-level messages */

/* Use the default kernel loglevel */
#define KERN_DEFAULT    "<d>"

/**
 * Annotation for a "continued" line of log printout (only done after a
 * line that had no enclosing \n). Only to be used by core/arch code
 * during early bootup (a continued line is not SMP-safe otherwise).
 */
#define KERN_CONT       "<c>"

extern int console_printk[];

#define console_loglevel            (console_printk[0])
#define default_message_loglevel    (console_printk[1])

/*---------- prototypes ----------*/
#ifdef CONFIG_PRINTK
int vprintk(const char *fmt, va_list args) __attribute__((format (printf, 1, 0)));
int printk(const char *fmt, ...) __attribute__((format (printf, 1, 2)));
#else
static inline int vprintk(const char *fmt, va_list args) __attribute__((format (printf, 1, 0)));
static inline int vprintk(const char *fmt, va_list args)
{
    return 0;
}
static inline int printk(const char *fmt, ...) __attribute__((format (printf, 1, 2)));
static inline int printk(const char *fmt, ...)
{
    return 0;
}
#endif

static inline int vscnprintf(char *buf, _size_t size, const char *fmt, va_list args)
{
    int len = 0;

    len = vsnprintf(buf, size, fmt, args);
    return ((len >= size) ? (size - 1) : len);
}

/**
 * container_of - cast a member of a structure out to the containing structure
 * @ptr:    the pointer to the member.
 * @type:   the type of the container struct this is embedded in.
 * @member: the name of the member within the struct.
 *
 */
#define container_of(ptr, type, member)     ({ \
                const typeof(((type *)0)->member) *__mptr = ptr; \
                (type *)((char *)__mptr - offsetof(type, member));})

#endif /* __KERNEL_H */
