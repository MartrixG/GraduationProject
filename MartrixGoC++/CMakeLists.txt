cmake_minimum_required(VERSION 3.19)
project(MartrixGoC++)

set(CMAKE_EXE_LINKER_FLAGS "-static")
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS -Wall)
set(CMAKE_CXX_FLAGS_DEBUG   "-g -Wall" )
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -Wall")

include_directories(${CMAKE_CURRENT_LIST_DIR}/include)

aux_source_directory(./src MARTRIX_GO_src)

link_libraries(ws2_32 wsock32)

add_executable(MartrixGoC++ ${MARTRIX_GO_src})