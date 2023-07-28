/**
 * @file      : linux_timer.h
 * @brief     : Linux平台定时器模块头文件
 * @author    : huenrong (huenrong1028@outlook.com)
 * @date      : 2023-07-13 20:15:01
 *
 * @copyright : Copyright (c) 2023 huenrong
 *
 * @history   : date        author          description
 *              2023-07-13  huenrong        创建文件
 *
 */

#ifndef __LINUX_TIMER_H
#define __LINUX_TIMER_H

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdint.h>
#include <stdbool.h>
#include <signal.h>
#include <time.h>

// 定时器对象结构体
struct _linux_timer_t;

/**
 * @brief  定时器回调函数
 * @param  linux_timer: 输入参数, 定时器对象
 */
typedef void (*linux_timer_cb_func)(const struct _linux_timer_t *linux_timer);

// 定时器对象结构体
typedef struct _linux_timer_t
{
    timer_t timer_id;             // 定时器ID
    linux_timer_cb_func timer_cb; // 定时器回调函数
    int32_t repeat_count;         // 定时器重复次数( -1: 无限循环; 1: 执行一次)
    const void *user_data;        // 用户数据
    uint32_t timeout;             // 定时器超时时间(单位: ms)
} linux_timer_t;

/**
 * @brief  创建定时器
 * @param  linux_timer: 输出参数, 定时器对象
 * @param  timer_cb   : 输入参数, 定时器回调函数
 * @param  timeout    : 输入参数, 定时器超时时间(单位: ms)
 * @param  user_data  : 输入参数, 用户数据
 * @return true : 成功
 * @return false: 失败
 */
bool linux_timer_create(linux_timer_t *linux_timer, const linux_timer_cb_func timer_cb, const uint32_t timeout,
                        const void *user_data);

/**
 * @brief  删除定时器
 * @param  linux_timer: 输出参数, 定时器对象
 * @return true : 成功
 * @return false: 失败
 */
bool linux_timer_delete(linux_timer_t *linux_timer);

/**
 * @brief  设置定时器回调函数
 * @param  linux_timer: 输出参数, 定时器对象
 * @param  timer_cb   : 输入参数, 定时器回调函数
 * @return true : 成功
 * @return false: 失败
 */
bool linux_timer_set_cb(linux_timer_t *linux_timer, const linux_timer_cb_func timer_cb);

/**
 * @brief  设置定时器超时时间
 * @param  linux_timer: 输出参数, 定时器对象
 * @param  timeout    : 输入参数, 定时器超时时间(单位: ms)
 * @return true : 成功
 * @return false: 失败
 */
bool linux_timer_set_timeout(linux_timer_t *linux_timer, const uint32_t timeout);

/**
 * @brief  设置定时器重复次数
 * @param  linux_timer : 输出参数, 定时器对象
 * @param  repeat_count: 输入参数, 定时器重复次数
 * @return true : 成功
 * @return false: 失败
 */
bool linux_timer_set_repeat_count(linux_timer_t *linux_timer, const int32_t repeat_count);

/**
 * @brief  设置定时器就绪(立即执行)
 * @param  linux_timer: 输出参数, 定时器对象
 * @return true : 成功
 * @return false: 失败
 */
bool linux_timer_ready(linux_timer_t *linux_timer);

#ifdef __cplusplus
}
#endif

#endif // __LINUX_TIMER_H
