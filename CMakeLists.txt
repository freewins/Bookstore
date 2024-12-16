cmake_minimum_required(VERSION 3.28)
project(Bookstrore)

set(CMAKE_CXX_STANDARD 20)
add_executable(Bookstore ./src/Bookstore.cpp
        src/book.cpp
        src/book.hpp
        src/file.cpp
        src/file.hpp
        src/logs.cpp
        src/logs.hpp
        src/user.cpp
        src/user.hpp
)
