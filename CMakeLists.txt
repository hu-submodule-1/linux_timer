cmake_minimum_required(VERSION 3.10)

# 定义静态库
add_library(linux_timer STATIC linux_timer.c)

# 指定需要链接的库
target_link_libraries(linux_timer PUBLIC rt)

# 添加头文件搜索路径
target_include_directories(linux_timer PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
