CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

PROJECT(test_webserver)
SET(CMAKE_CXX_FLAGS_DEBUG "-O0 -g ")
SET(CMAKE_CXX_STANDARD 11)
SET(CMAKE_BUILD_TYPE DEBUG)
SET(SRCS
    main.cpp
    web_server.cpp
    http_request.cpp
)

INCLUDE_DIRECTORIES(/usr/local/include)
ADD_EXECUTABLE(test_webserver ${SRCS})

LINK_DIRECTORIES(/usr/local/lib)
TARGET_LINK_LIBRARIES(test_webserver pthread)
