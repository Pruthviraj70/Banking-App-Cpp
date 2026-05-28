cmake_minimum_required(VERSION 4.2)
project(BankingAppCpp)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(BankingAppC__
        account.cpp
        account.h
        main.cpp)
