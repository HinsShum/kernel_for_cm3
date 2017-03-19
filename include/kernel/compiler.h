/**
 * /include/kernel/compiler.h
 *
 * Copyright (C) 2017 HinsShum
 *
 * compiler.h is free software: you can redistribute it and/or modify
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

#ifndef __COMPILER_H
#define __COMPILER_H

/*---------- marcos ----------*/
/**
 * GNU built-in functions
 */
#define likely(x)       __builtin_expect(!!(x), 1)
#define unlikely(x)     __builtin_expect(!!(x), 0)

/**
 * attribute
 */
#define __noreturn      __attribute__((noreturn))
#define __used          __attribute__((used))
#define __section(s)    __attribute__((section(#s)))

#endif /* __COMPILER_H */
