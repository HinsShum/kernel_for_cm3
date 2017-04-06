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
#define prefetch(x)     __builtin_prefetch(x)

/**
 * Force a compilation error if condition is true, but also
 * produce a result (of value 0 and type size_t), so the
 * expression can be used e.g. in a structure initializer (or
 * where-ever else comma expressions aren't permitted).
 */
#define BUILD_BUG_ON_ZERO(e)    (sizeof(struct { char bug[-!!e]; }))
#define BUILD_BUG_ON_NULL(e)    ((void *)sizeof(struct { char bug[-!!e]; }))

/* &a[0] degrades to a pointer: a different type from an array */
#define __must_be_array(a)      BUILD_BUG_ON_ZERO(__builtin_types_compatible_p(typeof(a), typeof(&a[0])))

/**
 * attribute
 */
#define __noreturn      __attribute__((noreturn))
#define __used          __attribute__((used))
#define __section(s)    __attribute__((section(#s)))
#define __must_check    __attribute__((warn_unused_result))

#endif /* __COMPILER_H */
