

#include <fcntl.h>
#include <string.h>
#include <sys/epoll.h>
#include <sys/sendfile.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>

#include "http_request.hpp"
#include "utility.hpp"
#include "web_server.hpp"

int HTTPRequest::handle_request(ClientRequest *request)
{
    request->code = HTTP_CODE::success_ok;
    if (handle_read(request) != HTTP_CODE::success_ok)
    {
        handle_error(request);
        return request->code;
    }

    if (request->head_position == request->tail_position)
    {
        // if empty body close connection
        handle_close(request);
        return request->code;
    }

    if (parse_request(request) != HTTP_CODE::success_ok)
    {
        handle_error(request);
        return request->code;
    }
    if (parse_headers(request) != HTTP_CODE::success_ok)
    {
        handle_error(request);
        return request->code;
    }

    if (handle_response(request) != HTTP_CODE::success_ok)
    {
        handle_error(request);
        return request->code;
    }

    // 更新连接状态
    epoll_event event;
    event.data.ptr = request;
    event.events = EPOLLIN | EPOLLET | EPOLLONESHOT;
    epoll_ctl(request->epoll_fd, EPOLL_CTL_MOD, request->fd, &event);

    LOG("code: %d, %s %s\n", request->code, request->method, request->uri);
    return request->code;
}

ssize_t HTTPRequest::handle_read(ClientRequest *request)
{
    // 将数据移动到最前面
    size_t length = request->tail_position - request->head_position;
    memmove(request->buffer, &request->buffer[request->head_position], length);

    // 从客户端socket读入数据到缓冲区
    size_t remain = REQUEST_BUFFER_SIZE - length;
    ssize_t size = read(request->fd, &request->buffer[length], remain - 1);

    request->tail_position = length + size;
    request->head_position = 0;

    request->buffer[request->tail_position] = 0;
    return request->code;
}

int HTTPRequest::parse_request(ClientRequest *request)
{
    char *buffer = request->buffer;
    int tail = request->tail_position;
    int position = request->head_position;

    // 解析method
    int index = 0;
    while (index < HTTP_METHOD_SIZE - 1 && position < tail && buffer[position] != ' ' && buffer[position] != '\0')
    {
        request->method[index++] = buffer[position++];
    }
    request->method[index] = '\0';

    // 解析uri
    index = 0;
    position++;
    while (index < REQUEST_URI_SIZE - 1 && position < tail && buffer[position] != ' ' && buffer[position] != '\0')
    {
        request->uri[index++] = buffer[position++];
    }
    CHECK_LOG_RETURN(index >= REQUEST_URI_SIZE,
                     request->code = HTTP_CODE::client_error_uri_too_long,
                     "414 URI Too Long\n");
    request->uri[index] = '\0';

    // 解析version
    index = 0;
    position++;
    while (index < HTTP_VERSION_SIZE - 1 && position < tail && buffer[position] != '\r' && buffer[position] != '\0')
    {
        request->version[index++] = buffer[position++];
    }
    request->version[index] = '\0';

    CHECK_LOG_RETURN(
        strcmp(request->version, "HTTP/1.0") && strcmp(request->version, "HTTP/1.1"),
        request->code = HTTP_CODE::server_error_http_version_not_supported,
        "505 HTTP Version Not Supported [%s]\n", request->version);

    // 读取最后一个 \n
    while (buffer[position] == '\n' && buffer[position] != '\0')
    {
        ++position;
    }

    request->head_position = position;
    return request->code;
}

int HTTPRequest::parse_headers(ClientRequest *request)
{
    std::string key = "";
    std::string value = "";
    char *buffer = request->buffer;
    size_t tail = request->tail_position;
    size_t position = request->head_position;

    while (position < tail && request->code == HTTP_CODE::success_ok)
    {
        while (position < tail && buffer[position] != ':')
        {
            key += buffer[position++];
        }

        ++position;
        while (position < tail && !(buffer[position] == '\r' && buffer[position + 1] == '\n'))
        {
            value += buffer[position++];
        }

        request->headers[strip(key, " \t\r\n")] = strip(value, " \t\r\n");
        key = value = "";

        position += 2;
        if (buffer[position] == '\r' && buffer[position + 1] == '\n')
        {
            position += 2;
            break;
        }
    }

    request->head_position = position;
    if (key.length() > 0 || value.length() > 0)
    {
        request->code = HTTP_CODE::client_error_bad_request;
    }

    return request->code;
}

int HTTPRequest::handle_error(ClientRequest *request)
{
    LOG("code: %d, %s %s\n", request->code, request->method, request->uri);
    if (request->code == HTTP_CODE::unknown)
    {
        return 0;
    }

    auto iter = HTTP_CODE_STRING.find(request->code);
    if (iter == HTTP_CODE_STRING.end())
    {
        iter = HTTP_CODE_STRING.find(HTTP_CODE::server_error_internal_server_error);
    }

    std::string buffer = "";
    buffer += std::string(request->version) + " " + iter->second + "\r\n";
    buffer += std::string("Date: ") + std::to_string(time(0)) + "\r\n";
    buffer += std::string("Server: ") + SERVER_NAME + "\r\n";
    buffer += std::string("Content-Length: 0\r\n");
    buffer += "\r\n";

    send(request->fd, buffer.c_str(), buffer.length(), 0);
    return 0;
}

int HTTPRequest::handle_close(ClientRequest *request)
{
    DEBUG_LOG("close socket: fd=%d", request->fd);
    close(request->fd);
    delete request;
    return 0;
}

int HTTPRequest::handle_response(ClientRequest *request)
{
    if (request->code != HTTP_CODE::success_ok)
    {
        return request->code;
    }

    // access and stat resource
    struct stat stat_file = {0};
    std::string strPath = std::string(request->sources_path) + request->uri;
    if (access(strPath.c_str(), F_OK) != 0)
    {
        DEBUG_LOG("path=%s not found.\n", strPath.c_str());
        request->code = HTTP_CODE::client_error_not_found;
        return request->code;
    }

    if (access(strPath.c_str(), R_OK) != 0)
    {
        DEBUG_LOG("path=%s access failed.\n", strPath.c_str());
        request->code = HTTP_CODE::client_error_forbidden;
        return request->code;
    }

    if (stat(strPath.c_str(), &stat_file) != 0 || !(stat_file.st_mode & S_IFREG))
    {
        DEBUG_LOG("path=%s is not a file, mode=%d.\n", strPath.c_str(), stat_file.st_mode);
        request->code = HTTP_CODE::client_error_forbidden;
        return request->code;
    }

    // add header data
    std::string buffer = "";
    buffer += std::string(request->version) + " 200 OK\r\n";
    buffer += std::string("Server: ") + SERVER_NAME + "\r\n";
    buffer += std::string("Date: ") + std::to_string(time(0)) + "\r\n";
    buffer += std::string("Last-Modified: ") + std::to_string(stat_file.st_mtime) + "\r\n";
    buffer += std::string("Content-Length: ") + std::to_string(stat_file.st_size) + "\r\n";

    // Connection
    auto header_iter = request->headers.find("Connection");
    if (header_iter != request->headers.end())
    {
        buffer += std::string("Connection: ") + header_iter->second + "\r\n";
    }

    // Content-Type
    std::string strType = strPath.substr(strPath.find_last_of("."));
    auto iter = MIME_TYPE_STRINGS.find(strType);
    if (iter != MIME_TYPE_STRINGS.end())
    {
        buffer += std::string("Content-Type: ") + iter->second + "\r\n";
    }
    else
    {
        buffer += std::string("Content-Type: ") + MIME_TYPE_STRINGS.find("default")->second + "\r\n";
    }

    buffer += "\r\n";

    // 发送响应头
    if (send(request->fd, buffer.c_str(), buffer.length(), 0) == -1)
    {
        LOG("send response header failed\n");
        request->code = HTTP_CODE::unknown;
        return request->code;
    }

    // 发送响应体
    int filefd = open(strPath.c_str(), O_RDONLY);
    if (filefd < 0)
    {
        LOG("read failed: path=%s%d\n", strPath.c_str());
        request->code = HTTP_CODE::server_error_internal_server_error;
    }
    else
    {
        sendfile(request->fd, filefd, NULL, stat_file.st_size);
    }

    return request->code;
}
