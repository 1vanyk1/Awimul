cmake_minimum_required(VERSION 3.10.2)
add_library( ipc
             SHARED
             ipc.cpp )
find_library(log-lib
             log )
target_link_libraries(ipc
            ${log-lib} )