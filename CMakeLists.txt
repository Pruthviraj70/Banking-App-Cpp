cmake_minimum_required(VERSION 4.2)
project(BankingAppCpp)

set(CMAKE_CXX_STANDARD 26)

include_directories(.)

find_package(OpenSSL REQUIRED)


add_executable(BankingAppC__
        account.cpp
        account.h
        main.cpp)

target_link_libraries(BankingAppC__ PRIVATE OpenSSL::SSL OpenSSL::Crypto)