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
/* 临时文件, 测试使用互斥量和二值信号量的printk, 将来要重新写 */
/*---------- includes ----------*/
#include <kernel/kernel.h>
#include <kernel/bug.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <plat/FreeRTOSConfig.h>
#include <mach/bsp_usart.h>

/*---------- functions ----------*/
extern void semaphore_mutex_init(void);
static int task_init(void)
{
    semaphore_mutex_init();
    
    return 0;
}

static void deamon_task(void *private)
{
    unsigned int i = 0;

    task_init();

    while(1){
        vTaskDelay(configTICK_RATE_HZ);     /* 2s */
        printk("HinsShum, second: %ds\n", i++);
    }
}

static inline void deamon_task_creat(void)
{
    xTaskCreate(deamon_task,
                "deamon",
                configMINIMAL_STACK_SIZE << 1,
                NULL,
                1,
                NULL);
}

/**
 * main() - C环境入口函数
 */
int __noreturn main(void)
{
    bsp_usart1_init();

    deamon_task_creat();

    /**
     * 启动任务调度
     */
    vTaskStartScheduler();
    
    while(1);
}
