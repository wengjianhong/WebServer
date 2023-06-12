/**
 * @file        reactor.hpp
 * @author      wengjianhong (wengjianhong2099@163.com)
 * @brief       Reactor反应堆模型
 * @version     0.1
 * @date        2023-05-23
 * @copyright   Copyright (c) 2023
 */
#ifndef __REACTOR_HPP__
#define __REACTOR_HPP__

#include "thread_pool.hpp"

class Reactor
{
protected:
    ThreadPool *m_pool = NULL;

public:
    Reactor(int pool_size) { m_pool = new ThreadPool(pool_size); }
    ~Reactor()
    {
        if (m_pool)
        {
            delete m_pool;
            m_pool = nullptr;
        }
    }

    // 处理分发请求
    virtual int handle_dispatch() = 0;
};

#endif
