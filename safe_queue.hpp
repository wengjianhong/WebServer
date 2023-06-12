/**
 * @file        safe_queue.hpp
 * @author      wengjianhong (wengjianhong2099@163.com)
 * @brief       线程安全队列
 * @version     0.1
 * @date        2023-05-18
 *
 * @copyright Copyright (c) 2023
 */

#ifndef __SAFE_QUEUE_HPP__
#define __SAFE_QUEUE_HPP__

#include <queue>
#include <mutex>

template <typename T>
class SafeQueue
{
private:
    /* 访问互斥信号量 */
    std::mutex m_mutex;
    /* 利用模板函数构造队列 */
    std::queue<T> m_queue;

public:
    SafeQueue() {}
    SafeQueue(SafeQueue &&other) {}
    ~SafeQueue() {}

    /* 返回队列是否为空 */
    bool empty()
    {
        std::unique_lock<std::mutex> lock(m_mutex);

        return m_queue.empty();
    }

    /* 获取队列元素个数 */
    int size()
    {
        std::unique_lock<std::mutex> lock(m_mutex);

        return m_queue.size();
    }

    /* 添加队列元素 */
    void enqueue(T &t)
    {
        std::unique_lock<std::mutex> lock(m_mutex);
        m_queue.emplace(t);
    }

    /* 取出队列元素 */
    bool dequeue(T &t)
    {
        std::unique_lock<std::mutex> lock(m_mutex);
        if (m_queue.empty())
        {
            return false;
        }

        /* 取出队首元素，返回队首元素值，并进行右值引用 */
        t = std::move(m_queue.front());
        m_queue.pop();
        return true;
    }
};

#endif
