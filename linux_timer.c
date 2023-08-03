/**
 * @file      : linux_timer.c
 * @brief     : Linux平台定时器模块源文件
 * @author    : huenrong (huenrong1028@outlook.com)
 * @date      : 2023-07-13 20:16:11
 *
 * @copyright : Copyright (c) 2023 huenrong
 *
 * @history   : date        author          description
 *              2023-07-13  huenrong        创建文件
 *
 */

#include <stdio.h>
#include <string.h>

#include "linux_timer.h"

/**
 * @brief  定时器回调线程
 * @param  sigev_value: 输入参数
 */
static void linux_timer_thread(union sigval sigev_value)
{
    linux_timer_t *linux_timer = (linux_timer_t *)sigev_value.sival_ptr;
    if (NULL == linux_timer)
    {
        return;
    }

    if (linux_timer->repeat_count > 0)
    {
        linux_timer->repeat_count--;
    }

    if (0 == linux_timer->repeat_count)
    {
        linux_timer_delete(linux_timer);
    }

    if (NULL != linux_timer->timer_cb)
    {
        linux_timer->timer_cb(linux_timer);
    }
}

/**
 * @brief  创建定时器
 * @param  linux_timer: 输出参数, 定时器对象
 * @param  timer_cb   : 输入参数, 定时器回调函数
 * @param  timeout    : 输入参数, 定时器超时时间(单位: ms)
 * @param  user_data  : 输入参数, 用户数据
 * @return true : 成功
 * @return false: 失败
 */
bool linux_timer_create(linux_timer_t *linux_timer, const linux_timer_cb timer_cb, const uint32_t timeout,
                        const void *user_data)
{
    struct sigevent sev = {0};
    memset(&sev, 0, sizeof(struct sigevent));
    sev.sigev_notify = SIGEV_THREAD;
    sev.sigev_notify_function = linux_timer_thread;
    sev.sigev_value.sival_ptr = linux_timer;
    // CLOCK_MONOTONIC: 获取的时间为系统重启到现在的时间, 更改系统时间对其没有影响
    if (-1 == timer_create(CLOCK_MONOTONIC, &sev, &linux_timer->timer_id))
    {
        return false;
    }

    linux_timer->timer_cb = timer_cb;
    linux_timer->repeat_count = -1;
    linux_timer->user_data = user_data;

    struct itimerspec timer_spec = {0};
    timer_spec.it_interval.tv_sec = (timeout / 1000);
    timer_spec.it_interval.tv_nsec = ((timeout % 1000) * 1000 * 1000);
    timer_spec.it_value.tv_sec = (timeout / 1000);
    timer_spec.it_value.tv_nsec = ((timeout % 1000) * 1000 * 1000);
    if (-1 == timer_settime(linux_timer->timer_id, 0, &timer_spec, NULL))
    {
        return false;
    }

    linux_timer->timeout = timeout;

    return true;
}

/**
 * @brief  删除定时器
 * @param  linux_timer: 输出参数, 定时器对象
 * @return true : 成功
 * @return false: 失败
 */
bool linux_timer_delete(linux_timer_t *linux_timer)
{
    if (!linux_timer->timer_id)
    {
        return true;
    }

    if (-1 == timer_delete(linux_timer->timer_id))
    {
        return false;
    }

    linux_timer->timer_id = NULL;

    return true;
}

/**
 * @brief  设置定时器回调函数
 * @param  linux_timer: 输出参数, 定时器对象
 * @param  timer_cb   : 输入参数, 定时器回调函数
 * @return true : 成功
 * @return false: 失败
 */
bool linux_timer_set_cb(linux_timer_t *linux_timer, const linux_timer_cb timer_cb)
{
    if (NULL == linux_timer)
    {
        return false;
    }

    linux_timer->timer_cb = timer_cb;

    return true;
}

/**
 * @brief  设置定时器超时时间
 * @param  linux_timer: 输出参数, 定时器对象
 * @param  timeout    : 输入参数, 定时器超时时间(单位: ms)
 * @return true : 成功
 * @return false: 失败
 */
bool linux_timer_set_timeout(linux_timer_t *linux_timer, const uint32_t timeout)
{
    if (NULL == linux_timer)
    {
        return false;
    }

    struct itimerspec timer_spec = {0};
    timer_spec.it_interval.tv_sec = (timeout / 1000);
    timer_spec.it_interval.tv_nsec = ((timeout % 1000) * 1000 * 1000);
    timer_spec.it_value.tv_sec = (timeout / 1000);
    timer_spec.it_value.tv_nsec = ((timeout % 1000) * 1000 * 1000);
    if (-1 == timer_settime(linux_timer->timer_id, 0, &timer_spec, NULL))
    {
        return false;
    }

    linux_timer->timeout = timeout;

    return true;
}

/**
 * @brief  设置定时器重复次数
 * @param  linux_timer : 输出参数, 定时器对象
 * @param  repeat_count: 输入参数, 定时器重复次数
 * @return true : 成功
 * @return false: 失败
 */
bool linux_timer_set_repeat_count(linux_timer_t *linux_timer, const int32_t repeat_count)
{
    if (NULL == linux_timer)
    {
        return false;
    }

    linux_timer->repeat_count = repeat_count;

    return true;
}

/**
 * @brief  设置定时器就绪(立即执行)
 * @param  linux_timer: 输出参数, 定时器对象
 * @return true : 成功
 * @return false: 失败
 */
bool linux_timer_ready(linux_timer_t *linux_timer)
{
    if (NULL == linux_timer)
    {
        return false;
    }

    struct itimerspec timer_spec = {0};
    timer_spec.it_interval.tv_sec = (linux_timer->timeout / 1000);
    timer_spec.it_interval.tv_nsec = ((linux_timer->timeout % 1000) * 1000 * 1000);
    timer_spec.it_value.tv_sec = 0;
    timer_spec.it_value.tv_nsec = 1;
    if (-1 == timer_settime(linux_timer->timer_id, 0, &timer_spec, NULL))
    {
        return false;
    }

    return true;
}

/**
 * @brief  暂停定时器
 * @param  linux_timer: 输出参数, 定时器对象
 * @return true : 成功
 * @return false: 失败
 */
bool linux_timer_pause(linux_timer_t *linux_timer)
{
    if (NULL == linux_timer)
    {
        return false;
    }

    struct itimerspec timer_spec = {0};
    if (-1 == timer_settime(linux_timer->timer_id, 0, &timer_spec, NULL))
    {
        return false;
    }

    return true;
}

/**
 * @brief  恢复定时器
 * @param  linux_timer: 输出参数, 定时器对象
 * @return true : 成功
 * @return false: 失败
 */
bool linux_timer_resume(linux_timer_t *linux_timer)
{
    if (NULL == linux_timer)
    {
        return false;
    }

    struct itimerspec timer_spec = {0};
    timer_spec.it_interval.tv_sec = (linux_timer->timeout / 1000);
    timer_spec.it_interval.tv_nsec = ((linux_timer->timeout % 1000) * 1000 * 1000);
    timer_spec.it_value.tv_sec = (linux_timer->timeout / 1000);
    timer_spec.it_value.tv_nsec = ((linux_timer->timeout % 1000) * 1000 * 1000);
    if (-1 == timer_settime(linux_timer->timer_id, 0, &timer_spec, NULL))
    {
        return false;
    }

    return true;
}
