cmake_minimum_required(VERSION 3.10)
project(qBlockServer)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 添加头文件路径
include_directories(
    ${PROJECT_SOURCE_DIR}
    ${PROJECT_SOURCE_DIR}/include
    ${PROJECT_SOURCE_DIR}/HttpServer/include
    /usr/include/mysql-cppconn-8
    /usr/include/mysql
    ${OPENSSL_INCLUDE_DIR}
)

find_library(MYSQLCPPCONN_LIBRARY
    NAMES mysqlcppconn mysqlcppconn8
    PATHS /usr/lib /usr/lib64 /usr/local/lib
)

# 添加所有源文件
file(GLOB_RECURSE HTTP_SERVER_SRC
    "${PROJECT_SOURCE_DIR}/HttpServer/src/*.cpp"
)

file(GLOB_RECURSE QBLOCK_SERVER_SRC
    "${PROJECT_SOURCE_DIR}/src/*.cpp"
)

set(MAIN_SRC "${PROJECT_SOURCE_DIR}/src/main.cpp")

add_executable(qBlockServer ${HTTP_SERVER_SRC} ${MAIN_SRC} ${QBLOCK_SERVER_SRC})

target_link_libraries(qBlockServer
    pthread
    muduo_net
    muduo_base
    mysqlcppconn
    mysqlclient
    ssl
    crypto
)

# 打印调试信息
message(STATUS "Include directories:")
get_property(dirs DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY INCLUDE_DIRECTORIES)
foreach(dir ${dirs})
    message(STATUS "  ${dir}")
endforeach()

message(STATUS "Main source file: ${MAIN_SRC}")
message(STATUS "HTTP Server sources: ${HTTP_SERVER_SRC}")