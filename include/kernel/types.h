/**
 * /include/kernel/types.h
 *
 * Copyright (C) 2017 HinsShum
 *
 * types.h is free software: you can redistribute it and/or modify
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

#ifndef _KERNEL_TYPES_H
#define _KERNEL_TYPES_H

/*---------- includes ----------*/
#include <asm/types.h>

/*---------- define data types ----------*/
typedef __int8_t        _int8_t;
typedef __uint8_t       _uint8_t;
typedef __int16_t       _int16_t;
typedef __uint16_t      _uint16_t;
typedef __int32_t       _int32_t;
typedef __uint32_t      _uint32_t;
typedef __int64_t       _int64_t;
typedef __uint64_t      _uint64_t;

typedef __kernel_size_t     _size_t;
typedef __kernel_sszie_t    _ssize_t;

#endif /* _KERNEL_TYPES_H */
