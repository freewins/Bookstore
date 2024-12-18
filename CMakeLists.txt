cmake_minimum_required(VERSION 3.28)
set(CMAKE_CXX_STANDARD 20)
project(code)

add_executable(code
        src/Bookstore.cpp)

target_include_directories(
        code
        PUBLIC
        include/
        src/
)







