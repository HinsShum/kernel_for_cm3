/**
 * /include/kernel/semaphore.h
 *
 * Copyright (C) 2017 HinsShum
 *
 * semaphore.h is free software: you can redistribute it and/or modify
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

#ifndef __SEMAPHORE_H
#define __SEMAPHORE_H

/*---------- includes ----------*/
#include <freertos/FreeRTOS.h>
#include <freertos/semphr.h>

/*---------- marcos ----------*/
#define semaphore_t     SemaphoreHandle_t
#define MAX_DELAY       portMAX_DELAY

#define DECLARE_SEMAPHORE(name)        SemaphoreHandle_t name = NULL

/*---------- functions ----------*/
/**
 * semaphore_init() - 创建计数信号量
 *
 * @param semaphore: 使用DECLARE_SEMAPHORE声明的变量的地址
 * @param max_count: 信号量支持的最大计数值
 * @param init_count: 信号量初始化时的计数值
 */
static inline void semaphore_init(semaphore_t *semaphore, unsigned int max_count, unsigned int init_count)
{
    *semaphore = xSemaphoreCreateCounting(max_count, init_count);
}

/**
 * semaphore_binary_init() - 创建二值信号量
 *
 * @param semaphore: 使用DECLARE_SEMAPHORE声明值的地址
 */
static inline void semaphore_binary_init(semaphore_t *semaphore)
{
    *semaphore = xSemaphoreCreateBinary();
}

/**
 * semaphore_down() - 获取信号量，不等待
 *
 * @param semaphore: 获取DECLARE_SEMAPHORE声明的变量
 * 的地址
 * Returns: 获取成功返回 1, 否则返回 0
 */
static inline int semaphore_down(semaphore_t *semaphore)
{
    return ((int)xSemaphoreTake(*semaphore, 0));
}

/**
 * semaphore_down_wait() - 获取信号量
 * 等待时间(time * systick)
 *
 * @param semaphore: 获取DECLARE_SEMAPHORE声明的变量
 * 的地址
 * @param time: 等待时间(time * systick)
 *
 * Returns: 获取成功返回 1, 否则返回 0
 */
static inline int semaphore_down_wait(semaphore_t *semaphore, unsigned int time)
{
    return ((int)xSemaphoreTake(*semaphore, 0));
}

/**
 * semaphore_down_blocked() - 获取信号量,
 * 永久等待直到信号量可用
 *
 * @param semaphore: 获取DECLARE_SEMAPHORE声明的变量
 * 的地址
 */
static inline void semaphore_down_blocked(semaphore_t *semaphore)
{
    xSemaphoreTake(*semaphore, MAX_DELAY);
}

/**
 * semaphore_up() - 释放信号量
 *
 * @param semaphore: 获取DECLARE_SEMAPHORE声明的变量
 * 的地址
 */
static inline void semaphore_up(semaphore_t *semaphore)
{
    xSemaphoreGive(*semaphore);
}

#undef semaphore_t
#undef MAX_DELAY

#endif /* __SEMAPHORE_H */
