cmake_minimum_required(VERSION 3.0)
#set(CMAKE_VERBOSE_MAKEFILE ON)

set(CMAKE_SYSTEM_NAME "Generic")

# delete cmake cache folder before changing this options
option(BUILD_SDL1 "Build with SDL2 support" OFF)
option(BUILD_SDL2 "Build with SDL2 support" OFF)
option(BUILD_PSP2 "Build with PSP2 support" OFF)
option(BUILD_NX "Build with SWITCH support" OFF)

# setup toolchain
include(Toolchain.cmake)

project(pTinyGL)

set(SDIR ${CMAKE_CURRENT_SOURCE_DIR})
set(BDIR ${CMAKE_CURRENT_BINARY_DIR})

set(FLAGS -Wall)
set(INCLUDES include)
file(GLOB SOURCES ${SDIR}/source/api/*.c)

if (BUILD_SDL1)
    list(APPEND SOURCES ${SDIR}/source/pgl_sdl1.c)
    list(APPEND FLAGS -DPNG_PATH=\"${SDIR}/examples/pfba.png\")
    set(LIBS SDL m)
elseif (BUILD_SDL2)
    list(APPEND SOURCES ${SDIR}/source/pgl_sdl2.c)
    list(APPEND FLAGS -DPNG_PATH=\"${SDIR}/examples/pfba.png\")
    set(LIBS SDL2 m)
elseif (BUILD_PSP2)
    list(APPEND SOURCES ${SDIR}/source/pgl_vita.c)
    list(APPEND FLAGS -DPNG_PATH=\"app0:pfba.png\")
    set(LIBS vita2d SceDisplay_stub SceGxm_stub SceSysmodule_stub SceCommonDialog_stub m)
elseif (BUILD_NX)
    list(APPEND SOURCES ${SDIR}/source/pgl_nx.c)
    list(APPEND FLAGS -DPNG_PATH=\"${SDIR}/examples/pfba.png\")
    set(LIBS ${DEVKITPRO}/libnx/lib/libnx.a m)
endif (BUILD_SDL1)

add_library(${PROJECT_NAME} ${SOURCES})
target_include_directories(${PROJECT_NAME} PRIVATE ${INCLUDES})
target_compile_options(${PROJECT_NAME} PRIVATE ${FLAGS})

add_executable(${PROJECT_NAME}.elf ${SDIR}/examples/example.c ${SDIR}/examples/lodepng.c)
target_include_directories(${PROJECT_NAME}.elf PRIVATE ${INCLUDES})
target_compile_options(${PROJECT_NAME}.elf PRIVATE ${FLAGS})
target_link_libraries(${PROJECT_NAME}.elf pTinyGL ${LIBS})
add_dependencies(${PROJECT_NAME}.elf ${PROJECT_NAME})

install(FILES ${CMAKE_BINARY_DIR}/libpTinyGL.a DESTINATION lib)
install(DIRECTORY ${CMAKE_SOURCE_DIR}/include/pTinyGL DESTINATION include)

############
# PSP2 VPK
############
if (BUILD_PSP2)
    vita_create_self(${PROJECT_NAME}.self ${PROJECT_NAME}.elf)
    vita_create_vpk(${PROJECT_NAME}.vpk "PTINYGL00" ${PROJECT_NAME}.self VERSION "01.00" NAME "pTinyGL")
endif (BUILD_PSP2)

##############
# SWITCH NRO
##############
if (BUILD_NX)
    set_target_properties(${PROJECT_NAME}.elf PROPERTIES LINK_FLAGS "-specs=switch.specs")
    add_custom_target(${PROJECT_NAME}.nro
            DEPENDS ${PROJECT_NAME}.elf
            COMMAND elf2nro ${PROJECT_NAME}.elf ${PROJECT_NAME}.nro
            )
endif (BUILD_NX)
