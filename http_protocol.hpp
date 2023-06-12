/**
 * @file        http_code.hpp
 * @author      wengjianhong (wengjianhong2099k@163.com)
 * @brief       http状态码
 * @version     0.1
 * @date        2023-05-29
 * @copyright   Copyright (c) 2023
 * @note        基于 https://github.com/eidheim/Simple-Web-Server/blob/master/status_code.hpp
 */

#ifndef __HTTP_PROTOCOL_HPP__
#define __HTTP_PROTOCOL_HPP__

#include <map>
#include <set>
#include <string>

enum HTTP_CODE
{
    information_continue = 100,
    information_switching_protocols,
    information_processing,
    success_ok = 200,
    success_created,
    success_accepted,
    success_non_authoritative_information,
    success_no_content,
    success_reset_content,
    success_partial_content,
    success_multi_status,
    success_already_reported,
    success_im_used = 226,
    redirection_multiple_choices = 300,
    redirection_moved_permanently,
    redirection_found,
    redirection_see_other,
    redirection_not_modified,
    redirection_use_proxy,
    redirection_switch_proxy,
    redirection_temporary_redirect,
    redirection_permanent_redirect,
    client_error_bad_request = 400,
    client_error_unauthorized,
    client_error_payment_required,
    client_error_forbidden,
    client_error_not_found,
    client_error_method_not_allowed,
    client_error_not_acceptable,
    client_error_proxy_authentication_required,
    client_error_request_timeout,
    client_error_conflict,
    client_error_gone,
    client_error_length_required,
    client_error_precondition_failed,
    client_error_payload_too_large,
    client_error_uri_too_long,
    client_error_unsupported_media_type,
    client_error_range_not_satisfiable,
    client_error_expectation_failed,
    client_error_im_a_teapot,
    client_error_misdirection_required = 421,
    client_error_unprocessable_entity,
    client_error_locked,
    client_error_failed_dependency,
    client_error_upgrade_required = 426,
    client_error_precondition_required = 428,
    client_error_too_many_requests,
    client_error_request_header_fields_too_large = 431,
    client_error_unavailable_for_legal_reasons = 451,
    server_error_internal_server_error = 500,
    server_error_not_implemented,
    server_error_bad_gateway,
    server_error_service_unavailable,
    server_error_gateway_timeout,
    server_error_http_version_not_supported,
    server_error_variant_also_negotiates,
    server_error_insufficient_storage,
    server_error_loop_detected,
    server_error_not_extended = 510,
    server_error_network_authentication_required,
    unknown,
};

enum HTTP_METHOD
{
    // http 1.0 支持  GET, POST, HEAD
    HTTP_GET = 0,
    HTTP_HEAD,
    HTTP_POST,

    // http 1.1 新增支持 PUT, PATCH, TRACE, DELETE, CONNECT, OPTIONS
    HTTP_PUT,
    HTTP_PATCH,
    HTTP_TRACE,
    HTTP_DELETE,
    HTTP_CONNECT,
    HTTP_OPTIONS,
};

static const std::set<std::string> HTTP_METHOD_STRINGS = {
    // http 1.0 支持  GET, POST, HEAD
    "GET",
    "HEAD",
    "POST",

    // http 1.1 新增支持 PUT, PATCH, TRACE, DELETE, CONNECT, OPTIONS
    "PUT",
    "PATCH",
    "TRACE",
    "DELETE",
    "CONNECT",
    "OPTIONS",
};

static const std::map<HTTP_METHOD, std::string> METHOD_STRINGS = {
    // http 1.0 支持  GET, POST, HEAD
    {HTTP_GET, "GET"},
    {HTTP_HEAD, "HEAD"},
    {HTTP_POST, "POST"},

    // http 1.1 新增支持 PUT, PATCH, TRACE, DELETE, CONNECT, OPTIONS
    {HTTP_PUT, "PUT"},
    {HTTP_PATCH, "PATCH"},
    {HTTP_TRACE, "TRACE"},
    {HTTP_DELETE, "DEELTE"},
    {HTTP_CONNECT, "CONNECT"},
    {HTTP_OPTIONS, "OPTIONS"},
};

static const std::map<HTTP_CODE, std::string> HTTP_CODE_STRING = {
    {HTTP_CODE::information_continue, "100 Continue"},
    {HTTP_CODE::information_switching_protocols, "101 Switching Protocols"},
    {HTTP_CODE::information_processing, "102 Processing"},
    {HTTP_CODE::success_ok, "200 OK"},
    {HTTP_CODE::success_created, "201 Created"},
    {HTTP_CODE::success_accepted, "202 Accepted"},
    {HTTP_CODE::success_non_authoritative_information, "203 Non-Authoritative Information"},
    {HTTP_CODE::success_no_content, "204 No Content"},
    {HTTP_CODE::success_reset_content, "205 Reset Content"},
    {HTTP_CODE::success_partial_content, "206 Partial Content"},
    {HTTP_CODE::success_multi_status, "207 Multi-Status"},
    {HTTP_CODE::success_already_reported, "208 Already Reported"},
    {HTTP_CODE::success_im_used, "226 IM Used"},
    {HTTP_CODE::redirection_multiple_choices, "300 Multiple Choices"},
    {HTTP_CODE::redirection_moved_permanently, "301 Moved Permanently"},
    {HTTP_CODE::redirection_found, "302 Found"},
    {HTTP_CODE::redirection_see_other, "303 See Other"},
    {HTTP_CODE::redirection_not_modified, "304 Not Modified"},
    {HTTP_CODE::redirection_use_proxy, "305 Use Proxy"},
    {HTTP_CODE::redirection_switch_proxy, "306 Switch Proxy"},
    {HTTP_CODE::redirection_temporary_redirect, "307 Temporary Redirect"},
    {HTTP_CODE::redirection_permanent_redirect, "308 Permanent Redirect"},
    {HTTP_CODE::client_error_bad_request, "400 Bad Request"},
    {HTTP_CODE::client_error_unauthorized, "401 Unauthorized"},
    {HTTP_CODE::client_error_payment_required, "402 Payment Required"},
    {HTTP_CODE::client_error_forbidden, "403 Forbidden"},
    {HTTP_CODE::client_error_not_found, "404 Not Found"},
    {HTTP_CODE::client_error_method_not_allowed, "405 Method Not Allowed"},
    {HTTP_CODE::client_error_not_acceptable, "406 Not Acceptable"},
    {HTTP_CODE::client_error_proxy_authentication_required, "407 Proxy Authentication Required"},
    {HTTP_CODE::client_error_request_timeout, "408 Request Timeout"},
    {HTTP_CODE::client_error_conflict, "409 Conflict"},
    {HTTP_CODE::client_error_gone, "410 Gone"},
    {HTTP_CODE::client_error_length_required, "411 Length Required"},
    {HTTP_CODE::client_error_precondition_failed, "412 Precondition Failed"},
    {HTTP_CODE::client_error_payload_too_large, "413 Payload Too Large"},
    {HTTP_CODE::client_error_uri_too_long, "414 URI Too Long"},
    {HTTP_CODE::client_error_unsupported_media_type, "415 Unsupported Media Type"},
    {HTTP_CODE::client_error_range_not_satisfiable, "416 Range Not Satisfiable"},
    {HTTP_CODE::client_error_expectation_failed, "417 Expectation Failed"},
    {HTTP_CODE::client_error_im_a_teapot, "418 I'm a teapot"},
    {HTTP_CODE::client_error_misdirection_required, "421 Misdirected Request"},
    {HTTP_CODE::client_error_unprocessable_entity, "422 Unprocessable Entity"},
    {HTTP_CODE::client_error_locked, "423 Locked"},
    {HTTP_CODE::client_error_failed_dependency, "424 Failed Dependency"},
    {HTTP_CODE::client_error_upgrade_required, "426 Upgrade Required"},
    {HTTP_CODE::client_error_precondition_required, "428 Precondition Required"},
    {HTTP_CODE::client_error_too_many_requests, "429 Too Many Requests"},
    {HTTP_CODE::client_error_request_header_fields_too_large, "431 Request Header Fields Too Large"},
    {HTTP_CODE::client_error_unavailable_for_legal_reasons, "451 Unavailable For Legal Reasons"},
    {HTTP_CODE::server_error_internal_server_error, "500 Internal Server Error"},
    {HTTP_CODE::server_error_not_implemented, "501 Not Implemented"},
    {HTTP_CODE::server_error_bad_gateway, "502 Bad Gateway"},
    {HTTP_CODE::server_error_service_unavailable, "503 Service Unavailable"},
    {HTTP_CODE::server_error_gateway_timeout, "504 Gateway Timeout"},
    {HTTP_CODE::server_error_http_version_not_supported, "505 HTTP Version Not Supported"},
    {HTTP_CODE::server_error_variant_also_negotiates, "506 Variant Also Negotiates"},
    {HTTP_CODE::server_error_insufficient_storage, "507 Insufficient Storage"},
    {HTTP_CODE::server_error_loop_detected, "508 Loop Detected"},
    {HTTP_CODE::server_error_not_extended, "510 Not Extended"},
    {HTTP_CODE::server_error_network_authentication_required, "511 Network Authentication Required"},
    {HTTP_CODE::unknown, "unknown code"},
};

static const std::map<std::string, std::string> MIME_TYPE_STRINGS = {
    {".mp3", "audio/mp3"},
    {".avi", "video/x-msvideo"},
    {".gz", "application/x-gzip"},
    {".doc", "application/msword"},
    {".xls", "application/vnd.ms-excel"},

    {".gif", "image/gif"},
    {".bmp", "image/bmp"},
    {".png", "image/png"},
    {".jpg", "image/jpeg"},
    {".ico", "image/x-icon"},

    {".c", "text/plain"},
    {".txt", "text/plain"},
    {".htm", "text/html"},
    {".html", "text/html"},
    {"default", "application/octet-stream"},
};

#endif // __HTTP_PROTOCOL_HPP__
