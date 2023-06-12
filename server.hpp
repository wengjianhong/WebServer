/**
 * @file        server.hpp
 * @author      wengjianhong (wengjianhong2099k@163.com)
 * @brief       常量和结构体定义
 * @version     0.1
 * @date        2023-05-27
 * @copyright   Copyright (c) 2023
 */

#ifndef __SERVER_HPP__
#define __SERVER_HPP__

#include <string.h>
#include <netinet/in.h>

#include "http_protocol.hpp"

static const int MAX_PATH = 1024;                       // max length of path string

static const char* SERVER_NAME = "www.pure-focus.top";  // 
static const char *SERVER_ADDRESS = "0.0.0.0";          // 服务端监听地址

static const int THREAD_POOL_SIZE = 32;                 // 线程池大小
static const int MAX_CLIENT_SIZE = 2048;                // 服务端最大连接数

static const int HTTP_METHOD_SIZE = 16;                 // HTTP请求方法长度
static const int HTTP_VERSION_SIZE = 16;                // HTTP版本号长度
static const int REQUEST_URI_SIZE = 2048;               // HTTP请求URI = 2KB
static const int REQUEST_BUFFER_SIZE = 50 << 10;        // HTTP请求缓冲大小 = 50KB


#define LOG(...) { printf("%64s:%-8d\t", __FILE__, __LINE__); printf(__VA_ARGS__); }
#define CHECK_LOG_RETURN(e, r, ...) do { if(e) { LOG(__VA_ARGS__); return r;}}while(0)
#define CHECK_LOG_CONTINUE(e, ...) {if(e) { LOG(__VA_ARGS__); continue; }}

// #define DEBUG

#ifdef DEBUG
#define DEBUG_LOG(...) { LOG(__VA_ARGS__); }
#else
#define DEBUG_LOG(...)
#endif // DEBUG


typedef struct RunParameters {
    int port;                                   // server port
    char path[MAX_PATH];                        // server data path

    RunParameters(){
        port = -1;
        memset(path, 0, sizeof(path));
    }
}RunParameters;

typedef struct ClientRequest
{
    int fd = -1;                                // client fd
    int epoll_fd = -1;                          // epoll fd
    const char* sources_path;                   // sources path

    HTTP_CODE code;                             // HTTP code
    char method[HTTP_METHOD_SIZE] = {0};        // HTTP method
    char version[HTTP_VERSION_SIZE] = {0};      // HTTP vestion

    size_t head_position = 0;                   // head position of buffer
    size_t tail_position = 0;                   // tail position of buffer (not included)
    char uri[REQUEST_URI_SIZE] = {0};           // request URI
    char buffer[REQUEST_BUFFER_SIZE] = {0};     // request buffer

    socklen_t addrlen = 0;                      // length of the socket address
    sockaddr client_addr = {0};                 // address of the client socket

    std::map<std::string, std::string> headers; // request header
} ClientRequest;

enum ServerState
{
    SERVER_STATE_INIT = 0,                      // server is init
    SERVER_STASTE_RUNNING,                      // server is running
    SERVER_STASTE_SUSPENDED,                    // server is suspended
    SERVER_STASTE_TERMINATED,                   // server is terminated
    SERVER_STASTE_ERROR,                        // server is error
    SERVER_STASTE_END                           // XXXX_END: 表示枚举变量的结束标志
};

#endif
