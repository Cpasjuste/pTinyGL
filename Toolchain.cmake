# setup toolchains

if (BUILD_PSP2)
    set(VITASDK $ENV{VITASDK})
    set(CMAKE_SYSTEM_PROCESSOR "armv7-a")
    set(CMAKE_C_COMPILER "${VITASDK}/bin/arm-vita-eabi-gcc")
    set(CMAKE_CXX_COMPILER "${VITASDK}/bin/arm-vita-eabi-g++")
    set(CMAKE_ASM_COMPILER "${VITASDK}/bin/arm-vita-eabi-gcc")
    set(CMAKE_C_FLAGS "-Wl,-q" CACHE STRING "C flags")
    set(CMAKE_CXX_FLAGS "${CMAKE_C_FLAGS} -std=gnu++11 -fpermissive" CACHE STRING "C++ flags")
    execute_process(COMMAND "date" "+%Y.%m.%d" OUTPUT_VARIABLE BUILD_DATE OUTPUT_STRIP_TRAILING_WHITESPACE)
    set(TITLE_ID "PTINYGL00")
endif (BUILD_PSP2)

if (BUILD_NX)
    set(DEVKITPRO $ENV{DEVKITPRO})
    set(CMAKE_SYSTEM_PROCESSOR "armv8-a")
    set(CMAKE_C_COMPILER "${DEVKITPRO}/devkitA64/bin/aarch64-none-elf-gcc")
    set(CMAKE_CXX_COMPILER "${DEVKITPRO}/devkitA64/bin/aarch64-none-elf-g++")
    set(CMAKE_ASM_COMPILER "${DEVKITPRO}/devkitA64/bin/aarch64-none-elf-as")
    set(CMAKE_AR "${DEVKITPRO}/devkitA64/bin/aarch64-none-elf-gcc-ar" CACHE STRING "")
    set(CMAKE_RANLIB "${DEVKITPRO}/devkitA64/bin/aarch64-none-elf-gcc-ranlib" CACHE STRING "")
    set(CMAKE_C_FLAGS "-march=armv8-a -mtp=soft -fPIE -ffast-math -ffunction-sections -fdata-sections -I${DEVKITPRO}/libnx/include -I${DEVKITPRO}/portlibs/armv8-a/include" CACHE STRING "C flags")
    set(CMAKE_CXX_FLAGS "${CMAKE_C_FLAGS} -fpermissive -fno-rtti -fno-exceptions -std=gnu++11" CACHE STRING "C++ flags")
    set(CMAKE_FIND_ROOT_PATH ${DEVKITPRO} ${DEVKITPRO}/devkitA64 ${DEVKITPRO}/libnx ${DEVKITPRO}/portlibs/armv8-a)
    set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
    set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
    set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
    set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)
    set(BUILD_SHARED_LIBS OFF CACHE INTERNAL "Shared libs not available")
endif (BUILD_NX)
