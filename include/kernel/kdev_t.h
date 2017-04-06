/**
 * /include/kernel/kdev_t.h
 *
 * Copyright (C) 2017 HinsShum
 *
 * kdev_t.h is free software: you can redistribute it and/or modify
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

#ifndef _KERNEL_KDEV_T_H
#define _KERNEL_KDEV_T_H

/*---------- marcos ----------*/
#define MINORBITS       (20)
#define MIMORMASK       ((1U << MINORBITS) - 1)

#define MAJOR(dev)      ((unsigned int)((dev) >> MINORBITS))
#define MINOR(dev)      ((unsigned int)((dev) & MIMORMASK))
#define MKDEV(ma, mi)   (((ma) << MINORBITS) | (mi))


#endif /* _KERNEL_KDEV_T_H */
