cmake_minimum_required(VERSION 4.2)
project(BankingAppCpp)

set(CMAKE_CXX_STANDARD 26)

include_directories(.)

add_library(
        accountLib
        account.cpp
)

find_package(OpenSSL REQUIRED)
find_package(SQLite3 REQUIRED)


add_executable(BankingAppC__
        main.cpp
        account.cpp
        src/sqlite3.c
        src/shell.c
        src/sqlite3.c
)

target_link_libraries(BankingAppC__
        PRIVATE
        SQLite3::SQLite3
        OpenSSL::SSL
        OpenSSL::Crypto
        accountLib
)