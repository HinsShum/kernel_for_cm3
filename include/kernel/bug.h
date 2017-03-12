/**
 * /include/kernel/bug.h
 *
 * Copyright (C) 2017 HinsShum
 *
 * bug.h is free software: you can redistribute it and/or modify
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

#ifndef __BUG_H
#define __BUG_H

/*---------- includes ----------*/
#include <kernel/compiler.h>

/*---------- functions ----------*/
static void __noreturn __bug(const char *file, const int line, const char *func)
{
    printk(KERN_CRIT "BUG: failure at %s: %d/%s()!\n", file, line, func);
    *(int *)0 = 0;
    /* Avoid "noreturn function does return" */
    for(; ;) {
    }
}

/*---------- marcos ----------*/
#define BUG()       __bug(__FILE__, __LINE__, __func__)

#define BUG_ON(condition)           \
    do {                            \
        if(unlikely(condition)) {   \
            BUG();                  \
        }                           \
    } while(0)

#endif /* __BUG_H */
