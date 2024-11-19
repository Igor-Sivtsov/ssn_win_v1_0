cmake_minimum_required(VERSION 3.16)

project(ssn_win_v1_0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(ssn_win_v1_0 main.cpp
    console.h)

include(GNUInstallDirs)
install(TARGETS ssn_win_v1_0
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
