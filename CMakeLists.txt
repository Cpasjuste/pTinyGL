cmake_minimum_required(VERSION 3.0)
#set(CMAKE_VERBOSE_MAKEFILE ON)

set(CMAKE_SYSTEM_NAME "Generic")
enable_language(ASM)

# delete cmake cache folder before changing this options
option(BUILD_SDL1 "Build with SDL2 support" OFF)
option(BUILD_SDL2 "Build with SDL2 support" OFF)
option(BUILD_PSP2 "Build with PSP2 support" OFF)
option(BUILD_NX "Build with SWITCH support" OFF)

# setup toolchain
include(Toolchain.cmake)

project(pTinyGL)

# flags
set(FLAGS -Wall)

set(INCLUDES include)

file(GLOB SOURCES source/api/*.c)

if (BUILD_SDL1)
    list(APPEND SOURCES source/pgl_sdl1.c)
    set(LIBS SDL m)
elseif (BUILD_SDL2)
    list(APPEND SOURCES source/pgl_sdl2.c)
    set(LIBS SDL2 m)
elseif (BUILD_NX)
    list(APPEND SOURCES source/pgl_nx.c)
    set(LIBS ${DEVKITPRO}/libnx/lib/libnx.a m)
endif (BUILD_SDL1)

add_library(${PROJECT_NAME} ${SOURCES})
target_include_directories(${PROJECT_NAME} PRIVATE ${INCLUDES})
target_compile_options(${PROJECT_NAME} PRIVATE ${FLAGS})

add_executable(${PROJECT_NAME}.elf examples/example.c examples/lodepng.c)
target_include_directories(${PROJECT_NAME}.elf PRIVATE ${INCLUDES})
target_compile_options(${PROJECT_NAME}.elf PRIVATE ${FLAGS})
target_link_libraries(${PROJECT_NAME}.elf pTinyGL ${LIBS})
add_dependencies(${PROJECT_NAME}.elf ${PROJECT_NAME})

if (BUILD_NX)
    set_target_properties(${PROJECT_NAME}.elf PROPERTIES LINK_FLAGS "-specs=switch.specs")
    add_custom_target(${PROJECT_NAME}.nro
            DEPENDS ${PROJECT_NAME}.elf
            COMMAND elf2nro ${PROJECT_NAME}.elf ${PROJECT_NAME}.nro
            )
endif (BUILD_NX)
