/**
 * @file        thread_pool.hpp
 * @author      wengjianhong (wengjianhong2099@163.com)
 * @brief       线程池
 * @version     0.1
 * @date        2023-05-18
 * @copyright   Copyright (c) 2023
 */

/* 参考：https://zhuanlan.zhihu.com/p/367309864 */

#ifndef __THREAD_POOL_HPP__
#define __THREAD_POOL_HPP__

#include <condition_variable>
#include <future>
#include <mutex>
#include <queue>
#include <string>
#include <thread>
#include <utility>
#include <vector>

#include "safe_queue.hpp"

#define STATE_PERFORM_TASK (0x01) /* Performs tasks */

class ThreadPool
{
    ThreadPool(const ThreadPool &) = delete;
    ThreadPool &operator=(const ThreadPool &) = delete;

public:
    explicit ThreadPool(int thread_num) : m_thread_num(thread_num)
    {
        m_worker_threads = std::vector<std::thread>(m_thread_num);
    }
    ~ThreadPool()
    {
        stop();
    }

    /* 开始运行 */
    int start()
    {
        if (m_state & STATE_PERFORM_TASK)
        {
            return 0;
        }
        if (m_worker_threads.size() != m_thread_num)
        {
            return -1;
        }

        m_state |= STATE_PERFORM_TASK;
        for (int i = 0; i < m_thread_num; ++i)
        {
            m_worker_threads.at(i) = std::move(std::thread(&ThreadPool::thread_task, this));
        }
        m_condition_lock.notify_all();
        return 0;
    }

    /* 停止未开始任务 */
    void stop()
    {
        m_state &= (~STATE_PERFORM_TASK);
        m_condition_lock.notify_all();
        for (int i = 0; i < m_worker_threads.size(); ++i)
        {
            if (m_worker_threads.at(i).joinable())
            {
                m_worker_threads.at(i).join();
            }
        }
    }

    /* 向任务队列中新增一个任务（任务函数返回值必须为int） */
    template <typename Fun, typename... Args>
    std::future<int> enqueue(Fun &&f, Args &&...args)
    {
        std::function<int()> func = std::bind(std::forward<Fun>(f), std::forward<Args>(args)...);
        auto task_ptr = std::make_shared<std::packaged_task<int()>>(func);
        std::function<void()> wrapper_func = [task_ptr]()
        {
            (*task_ptr)();
        };

        m_task_queue.enqueue(wrapper_func);
        m_condition_lock.notify_one();
        return task_ptr->get_future();
    }

    /* 获取初始化线程个数 */
    uint size() { return m_thread_num; }

private:
    /* 线程的处理任务函数 */
    void thread_task()
    {
        bool flag = false; // 标记是否获取到任务
        std::function<void()> task_func;

        /* 从任务队列中获取并执行任务 */
        while (m_state & STATE_PERFORM_TASK)
        {
            {
                std::unique_lock<std::mutex> lock(m_conditional_mutex);
                if (m_task_queue.size() == 0)
                {
                    m_condition_lock.wait(lock);
                }

                flag = m_task_queue.dequeue(task_func);
            }

            if (flag == true)
            {
                task_func();
            }
        }
    }

private:
    /* 运行状态 */
    int m_state = 0;
    /* 线程个数 */
    int m_thread_num = 0;
    /* 线程数组 */
    std::vector<std::thread> m_worker_threads;
    /* 任务队列 */
    SafeQueue<std::function<void()>> m_task_queue;

    /* 工作状态互斥量 */
    std::mutex m_conditional_mutex;
    /* 工作状态条件变量 */
    std::condition_variable m_condition_lock;
};

#endif
