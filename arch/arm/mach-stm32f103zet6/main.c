/**
 * /arch/arm/mach-stm32f103zet6/main.c
 *
 * Copyright (C) 2017 HinsShum
 *
 * main.c is free software: you can redistribute it and/or modify
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

/*---------- includes ----------*/
#include <kernel/kernel.h>
#include <kernel/bug.h>
#include <mach/bsp_usart.h>


int __noreturn main(void)
{
    bsp_usart1_init();
    printk(KERN_ERR "System start...\n");
    while(1) {
        for(int volatile i = 0; i < 0xFFFFFF; i++);
        printk(KERN_ERR "HinsShum\n");
    }
}
