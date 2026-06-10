cmake_minimum_required(VERSION 3.20)
project(BankingAppCpp)

set(CMAKE_CXX_STANDARD 26)

include_directories(.)

find_package(OpenSSL REQUIRED)

add_library(
        accountLib
        account.cpp
)

target_link_libraries(
        accountLib
        PUBLIC
        OpenSSL::SSL
        OpenSSL::Crypto
)

add_executable(
        BankingAppC__
        main.cpp
        src/sqlite3.c
)

target_link_libraries(
        BankingAppC__
        PRIVATE
        accountLib
)