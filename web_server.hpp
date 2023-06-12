/**
 * @file        webserver.hpp
 * @author      wengjianhong (wengjianhong2099@163.com)
 * @brief       WebServer类实现
 * @version     0.1
 * @date        2023-05-23
 * @copyright   Copyright (c) 2023
 */

#ifndef __WEB_SERVER_HPP__
#define __WEB_SERVER_HPP__

#include <sys/socket.h>

#include "server.hpp"
#include "reactor.hpp"
#include "http_request.hpp"

class WebServer : public Reactor
{
    char m_sz_sources_path[MAX_PATH]; // web资源目录

    int m_fd_epoll;                  // epoll句柄
    int m_fd_listener;               // 监听句柄
    int m_num_server_port;           // 监听端口
    int m_num_client_size;           // 最大连接个数
    int m_num_threadpool_sizes;      // 线程池大小
    ServerState m_num_states;        // 状态
    struct epoll_event *m_ptr_event; // 接收epoll_wait的发生事件的数组指针

private:
    // 分发事件
    int handle_dispatch() override;

private:
    // 初始化服务
    int server_init();
    // 监听服务端口
    int server_listen();
    // 处理连接
    int handle_accept();

public:
    WebServer(int server_port, const char* sources_path, int client_size, int pool_size);
    ~WebServer();

    int start();
    int suspend();
    int terminate();

    int get_server_port() { return m_num_server_port; }
    bool is_running() { return m_num_states == ServerState::SERVER_STASTE_RUNNING; }

public:
    const char *get_sources_path() { return m_sz_sources_path; }
    const char *set_sources_path(const char *path) { strncpy(m_sz_sources_path, path, sizeof(m_sz_sources_path)); }
};

#endif
