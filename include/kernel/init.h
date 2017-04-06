/**
 * /include/kernel/init.h
 *
 * Copyright (C) 2017 HinsShum
 *
 * init.h is free software: you can redistribute it and/or modify
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

#ifndef __INIT_H
#define __INIT_H

/*---------- inclides ----------*/
#include <kernel/compiler.h>

/*---------- marcos ----------*/
#define __init          __section(init_text)
#define __exit_call     __used __section(exitcall_exit)

/**
 * Used for initialization calls..
 */
typedef int (*initcall_t)(void);
typedef void (*exitcall_t)(void);

/**
 * The `id' arg to __define_initcall() is needed so that
 * multiple initcalls can point at the same handler without
 * causing duplicate-symbol build errors.
 */
#define __define_initcall(level, fn, id) \
    static initcall_t __initcall_##fn##id __used \
    __attribute__((section("initcall" level "_init"))) = fn

#define pure_initcall(fn)       __define_initcall("0", fn, 0)
#define core_initcall(fn)       __define_initcall("1", fn, 1)
#define postcore_initcall(fn)   __define_initcall("2", fn, 2)
#define arch_initcall(fn)       __define_initcall("3", fn, 3)
#define subsys_initcall(fn)     __define_initcall("4", fn, 4)
#define fs_initcall(fn)         __define_initcall("5", fn, 5)
#define module_initcall(fn)     __define_initcall("6", fn, 6)
#define late_initcall(fn)       __define_initcall("7", fn, 7)

#define __initcall(fn)          device_initcall(fn)
#define __exitcall(fn)  \
        static exitcall_t __exitcall_##fn __exit_call = fn

/**
 * module_init() - driver initialization entry point
 * @x: function to be run at kernel boot time or module insertion
 *
 * module_init() will either be called during do_initcalls() (if
 * builtin) or at module insertion time (if a module).  There can only
 * be one per module.
 */
#define module_init(x)     __initcall(x)

/**
 * module_exit() - driver exit entry point
 * @x: function to be run when driver is removed
 *
 * module_exit() will wrap the driver clean-up code
 * with cleanup_module() when used with rmmod when
 * the driver is a module.  If the driver is statically
 * compiled into the kernel, module_exit() has no effect.
 * There can only be one per module.
 */
#define module_exit(x)      __exitcall(x)

#endif /* __INIT_H */
