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

############
# PSP2 VPK
############
if (BUILD_PSP2)
    add_custom_target(${PROJECT_NAME}.vpk
            DEPENDS ${PROJECT_NAME}.elf
            COMMAND rm -rf ${BDIR}/vpk && mkdir -p ${BDIR}/vpk/sce_sys
            COMMAND ${VITASDK}/bin/vita-elf-create ${PROJECT_NAME}.elf ${PROJECT_NAME}.velf
            COMMAND ${VITASDK}/bin/vita-make-fself -c ${PROJECT_NAME}.velf ${BDIR}/vpk/eboot.bin
            COMMAND ${VITASDK}/bin/vita-mksfoex -s TITLE_ID=${TITLE_ID} "${PROJECT_NAME}" ${BDIR}/vpk/sce_sys/param.sfo
            COMMAND cp ${SDIR}/examples/pfba.png ${BDIR}/vpk/
            COMMAND cd ${BDIR}/vpk && zip -r ../${PROJECT_NAME}-${BUILD_DATE}.vpk . && cd ${BDIR}
            )
    add_dependencies(${PROJECT_NAME}.vpk ${PROJECT_NAME}.elf)
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
