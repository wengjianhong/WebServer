/**
 * @file        utility.hpp
 * @author      wengjianhong (wengjianhong2099k@163.com)
 * @brief       工具函数
 * @version     0.1
 * @date        2023-05-29
 * @copyright   Copyright (c) 2023
 * @note        基于 https://github.com/eidheim/Simple-Web-Server
 */

#ifndef __UTILITY_HPP__
#define __UTILITY_HPP__

#include <string>

/**
 * @brief               从'源字符串'头尾删除任一出现在'清除串'中的字符
 *
 * @param str           源字符串
 * @param delim         清除串
 * @return std::string  结果字符串
 */
std::string strip(const std::string & str, const std::string & delim){
    std::string result = str;
    result.erase(0, result.find_first_not_of(delim));
    result.erase(result.find_last_not_of(delim) + 1);
    return std::move(result);
}

#endif // __UTILITY_HPP__
