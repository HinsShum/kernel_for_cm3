/**
 * /include/kernel/mutex.h
 *
 * Copyright (C) 2017 HinsShum
 *
 * mutex.h is free software: you can redistribute it and/or modify
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

#ifndef __MUTEX_H
#define __MUTEX_H

/*---------- includes ----------*/
#include <freertos/FreeRTOS.h>
#include <freertos/semphr.h>

/*---------- marcos ----------*/
#define mutex_t     SemaphoreHandle_t
#define MAX_DELAY   portMAX_DELAY

#define DECLARE_MUTEX(name)        SemaphoreHandle_t name = NULL

/*---------- functions ---------*/
/**
 * mutex_init() - 创建互斥信号量
 *
 * @param  mutex: 使用DECLARE_MUTEX声明的变量的地址
 */
static inline void mutex_init(mutex_t *mutex)
{
    *mutex = xSemaphoreCreateMutex();
}

/**
 * mutex_lock() - 申请获得互斥信号量, 不等待
 *
 * @param  mutex: 使用DECLARE_MUTEX声明的变量的地址
 *
 * Returns: 成功获得返回 1, 否则返回 0
 */
static inline int mutex_lock(mutex_t *mutex)
{
    return ((int)xSemaphoreTake(*mutex, 0));
}

/**
 * mutex_lock_wait() - 申请获得互斥信号量,
 * 等待时间为 (time * systick)
 *
 * @param  mutex: 使用DECLARE_MUTEX声明的变量的地址
 * @param  time: 等待时间 (time * systick)
 *
 * Returns: 成功获得返回 1, 否则返回 0
 */
static inline int mutex_lock_wait(mutex_t *mutex, unsigned int time)
{
    return ((int)xSemaphoreTake(*mutex, time));
}

/**
 * mutex_lock_blocked() - 申请获得互斥信号量,
 * 永久等待知道信号量可用
 *
 * @param  mutex: 使用DECLARE_MUTEX声明的变量的地址
 */
static inline void mutex_lock_blocked(mutex_t *mutex)
{
    xSemaphoreTake(*mutex, MAX_DELAY);
}

/**
 * mutex_unlock() - 释放获取的互斥信号量
 *
 * @param  mutex: 使用DECLARE_MUTEX声明的变量的地址
 */
static inline void mutex_unlock(mutex_t *mutex)
{
    xSemaphoreGive(*mutex);
}

#undef mutex_t
#undef MAX_DELAY

#endif /* __MUTEX_H */
