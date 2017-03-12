/**
 * /arch/arm/include/asm/types.h
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

#ifndef _ASM_ARM_TYPES_H
#define _ASM_ARM_TYPES_H

/**
 * This file is generally used by user-level software, so you
 * need to be a little careful about namespace pollution etc.
 */
typedef char                __int8_t;
typedef unsigned char       __uint8_t;
typedef short               __int16_t;
typedef unsigned short      __uint16_t;
typedef int                 __int32_t;
typedef unsigned int        __uint32_t;
typedef long long           __int64_t;
typedef unsigned long long  __uint64_t;


typedef int                 __kernel_sszie_t;
typedef unsigned int        __kernel_size_t;

#endif /* _ASM_ARM_TYPES_H */
