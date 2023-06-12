/**
 * @file        http_request.hpp
 * @author      wengjianhong (wengjianhong2099@163.com)
 * @brief       HTTP请求
 * @version     0.1
 * @date        2023-05-30
 * @copyright   Copyright (c) 2023
 */
#ifndef __HTTP_H__
#define __HTTP_H__

#include <stdio.h>
#include <string.h>

#include "server.hpp"
#include "http_protocol.hpp"


class HTTPRequest
{
public:
    // 处理客户端请求
    static int handle_request(ClientRequest *request);

private:
    // 从客户端读入请求数据
    static ssize_t handle_read(ClientRequest *request);

    // 解析请求行, 包含: method, uri, version
    static int parse_request(ClientRequest *request);
    // 解析请求头
    static int parse_headers(ClientRequest *request);

    // 关闭连接
    static int handle_close(ClientRequest *request);
    // 处理错误
    static int handle_error(ClientRequest *request);
    // 向客户端发送响应数据
    static int handle_response(ClientRequest *request);

    // 构造响应体
    static int generate_response(ClientRequest *request);
};

#endif
