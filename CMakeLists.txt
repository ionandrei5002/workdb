project(workdb)
cmake_minimum_required(VERSION 2.8)

#set(CMAKE_VERBOSE_MAKEFILE ON)

set(HEADERS src/base.h src/buffer.h src/column.h src/dataset.h src/definitions.h src/primitive.h src/printer.h src/rawbuffer.h src/reader.h src/row.h src/schema.h src/table.h src/types.h src/writeraheadlogger.h src/allocator/bitmap_allocator.h src/allocator/bitmap_pool.h src/allocator/free_list_allocator.h src/allocator/free_list_pool.h src/allocator/pool.h src/allocator/pool_interface.h)
set(SOURCES ${HEADERS} src/main.cpp src/buffer.cpp src/column.cpp src/printer.cpp src/rawbuffer.cpp src/reader.cpp src/row.cpp src/table.cpp src/writeraheadlogger.cpp)

add_executable(${PROJECT_NAME} ${SOURCES})

target_link_libraries(${PROJECT_NAME} snappy)

target_compile_options(${PROJECT_NAME}
  PRIVATE
    -flto
    -march=native
    -std=c++17
    -O3
    -g
#    -m64
#    -Werror
#    -Wno-unused-parameter
#    -Wfatal-errors
#    -pedantic-errors
#    -Wall
#    -Wno-missing-braces
#    -Wcast-align
#    -Wcast-qual
#    -Wctor-dtor-privacy
#    -Wextra
#    -Wformat=2
#    -Wmissing-declarations
#    -Wno-long-long
#    -Wnon-virtual-dtor
#    -Woverlength-strings
#    -Woverloaded-virtual
#    -Wredundant-decls
#    -Wreorder
#    -Wswitch-default
#    -Wunused-variable
#    -Wwrite-strings
#    -Wno-variadic-macros
#    -Wno-unused-local-typedef
#    -Wno-language-extension-token
#    -fstrict-aliasing
#    -Wconversion
#    -Wfloat-equal
#    -Wold-style-cast
#    -Wunreachable-code
#    -Wshadow
#    -Wpointer-arith
#    -Wcast-align
#    -Wformat-nonliteral
#    -Winit-self
    -floop-parallelize-all
    -ftree-parallelize-loops=4
)
