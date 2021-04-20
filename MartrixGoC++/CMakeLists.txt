cmake_minimum_required(VERSION 3.19)
project(MartrixGoC++)

set(CMAKE_EXE_LINKER_FLAGS "-static-libgcc -static-libstdc++")
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS -g)
set(CMAKE_CXX_FLAGS -Wall)
# set(CMAKE_CXX_FLAGS_RELEASE "-O2 -DNDEBUG")
# 调试时开启这个命令
SET(CMAKE_BUILD_TYPE Debug)

include_directories(${CMAKE_CURRENT_LIST_DIR}/include)

aux_source_directory(./src MARTRIX_GO_src)

# link_directories(${CMAKE_CURRENT_LIST_DIR}/lib)

add_executable(MartrixGoC++ ${MARTRIX_GO_src})
