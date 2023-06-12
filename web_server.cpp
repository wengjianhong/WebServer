#include "web_server.hpp"

#include <unistd.h>
#include <string.h>
#include <sys/epoll.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

WebServer::WebServer(int server_port, const char *sources_path, int client_size, int pool_size)
    : Reactor(pool_size)
{
    m_num_server_port = server_port;
    m_num_client_size = client_size;
    m_num_threadpool_sizes = pool_size;

    m_fd_epoll = -1;
    m_fd_listener = -1;
    m_num_states = ServerState::SERVER_STATE_INIT;

    m_ptr_event = nullptr;
    strncpy(m_sz_sources_path, sources_path, sizeof(m_sz_sources_path));
}

WebServer::~WebServer()
{
    if (m_ptr_event != nullptr)
    {
        delete[] m_ptr_event;
        m_ptr_event = nullptr;
    }
}

// private member function

int WebServer::server_init()
{
    m_fd_epoll = epoll_create(MAX_CLIENT_SIZE);
    CHECK_LOG_RETURN(m_fd_epoll <= 0, -1, "init() failed: init epoll failed");

    m_ptr_event = new epoll_event[m_num_client_size];
    CHECK_LOG_RETURN(m_ptr_event == nullptr, -1, "init() failed: init event array failed");

    return 0;
}

int WebServer::server_listen()
{
    /* 创建socket */
    m_fd_listener = socket(AF_INET, SOCK_STREAM, 0);
    CHECK_LOG_RETURN(m_fd_listener == -1, -1, "socket() failed");

    /* 设置服务端监听IP和接口 */
    sockaddr_in ser_addr = {0};
    ser_addr.sin_family = AF_INET;
    ser_addr.sin_port = htons(m_num_server_port);
    ser_addr.sin_addr.s_addr = inet_addr(SERVER_ADDRESS);

    /* 绑定端口 */
    int error_no = bind(m_fd_listener, (sockaddr *)&ser_addr, sizeof(ser_addr));
    CHECK_LOG_RETURN(error_no != 0, error_no, "bind error, code=%d\n", error_no);

    /* 监听端口 */
    error_no = listen(m_fd_listener, 5);
    CHECK_LOG_RETURN(error_no != 0, error_no, "listen error, code=%d\n", error_no);

    return 0;
}

int WebServer::handle_accept()
{
    socklen_t addrlen = 0;
    sockaddr client_addr = {0};
    while (m_num_states == ServerState::SERVER_STASTE_RUNNING)
    {
        int client_fd = accept(m_fd_listener, &client_addr, &addrlen);
        if (client_fd < 0)
        {
            DEBUG_LOG("accept() error\n");
            continue;
        }

        // 加到epoll
        ClientRequest *request = new ClientRequest();
        CHECK_LOG_CONTINUE(request == nullptr, "accept() error: client_fd = %d, code = -1\n", client_fd);

        memcpy(&request->client_addr, &client_addr, sizeof(client_addr));
        request->sources_path = m_sz_sources_path;
        request->epoll_fd = m_fd_epoll;
        request->addrlen = addrlen;
        request->fd = client_fd;

        struct epoll_event event;
        event.data.ptr = request;
        event.events = EPOLLIN | EPOLLET | EPOLLONESHOT;
        if (epoll_ctl(m_fd_epoll, EPOLL_CTL_ADD, client_fd, &event) < 0)
        {
            DEBUG_LOG("epoll_add error\n");
            continue;
        }
    }
}

int WebServer::handle_dispatch()
{
    int error_no = 0;
    int event_num = 0;
    struct epoll_event *event = NULL;
    while (m_num_states & SERVER_STASTE_RUNNING)
    {
        event_num = epoll_wait(m_fd_epoll, m_ptr_event, m_num_client_size, -1);

        for (int i = 0; i < event_num; i++)
        {
            // 移除错误事件
            event = &m_ptr_event[i];
            ClientRequest *request = (ClientRequest *)(event->data.ptr);

            if (request != NULL && (event->events & EPOLLERR) || (event->events & EPOLLHUP))
            {
                epoll_ctl(m_fd_epoll, EPOLL_CTL_DEL, event->data.fd, nullptr);
                DEBUG_LOG("remove error event: fd=%d\n", event->data.fd);
                close(event->data.fd);
                delete request;
                continue;
            }

            // 把所有请求 放到任务队列
            if (request != NULL && event->events & EPOLLIN)
            {
                (void)m_pool->enqueue(&HTTPRequest::handle_request, request);
            }
        }
    }
}

// public member function

int WebServer::start()
{
    if (server_init() != 0 || server_listen() != 0)
    {
        return -1;
    }

    int ret = m_pool->start();
    CHECK_LOG_RETURN(ret != 0, -1, "start error, code=%d\n", ret);

    m_num_states = ServerState::SERVER_STASTE_RUNNING;
    (void)m_pool->enqueue(&WebServer::handle_accept, this);
    (void)m_pool->enqueue(&WebServer::handle_dispatch, this);

    return 0;
}

int WebServer::suspend()
{
    m_num_states = ServerState::SERVER_STASTE_SUSPENDED;
    return 0;
}

int WebServer::terminate()
{
    m_num_states = ServerState::SERVER_STASTE_TERMINATED;
    return 0;
}
