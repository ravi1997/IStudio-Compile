cmake_minimum_required(VERSION 3.26)

PROJECT(
    IStudio
    VERSION 1.0.1
    LANGUAGES CXX
)


# Enable CCache if available
find_program(CCACHE_PROGRAM ccache)
if(CCACHE_PROGRAM)
    message(STATUS "Found ccache: ${CCACHE_PROGRAM}")
    set(CMAKE_C_COMPILER_LAUNCHER ${CCACHE_PROGRAM})
    set(CMAKE_CXX_COMPILER_LAUNCHER ${CCACHE_PROGRAM})
endif()


set(CMAKE_CXX_STANDARD 23)
#set(CMAKE_BUILD_TYPE "Debug")
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/externals/sanitizers-cmake/cmake" ${CMAKE_MODULE_PATH})
find_package(Sanitizers MODULE REQUIRED)

#set(CMAKE_CXX_CLANG_TIDY clang-tidy --checks=-*,cppcoreguidelines-*)
add_compile_options(-pedantic -Wall -Wextra -g)
set(CMAKE_BUILD_PARALLEL_LEVEL 8)  # Adjust based on your CPU cores


add_definitions(-D__DATE__="May 14 2021")
add_definitions(-D__TIME__="17:11:43")
add_compile_options(-Wno-builtin-macro-redefined)


set_property(GLOBAL PROPERTY JOB_POOLS compile=16 link=1 codegen=16)
add_subdirectory(src)
add_subdirectory(include)
add_executable(${PROJECT_NAME} ${SRC_FILES})

set_property(TARGET ${PROJECT_NAME} PROPERTY JOB_POOL_COMPILE compile)
set_property(TARGET ${PROJECT_NAME} PROPERTY JOB_POOL_LINK link)
#target_link_libraries(${PROJECT_NAME} Dbghelp)


add_sanitizers(${PROJECT_NAME})
target_include_directories(${PROJECT_NAME} PRIVATE ${INC_DIRS})

install(TARGETS ${PROJECT_NAME}
        DESTINATION /usr/local/bin)

# Check compiler and set module flags
if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    # GCC
    target_compile_options(${PROJECT_NAME} PRIVATE -fmodules-ts)
    target_link_options(${PROJECT_NAME} PRIVATE -fmodules-ts)
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    # Clang
    target_compile_options(${PROJECT_NAME} PRIVATE -fmodules)
    target_link_options(${PROJECT_NAME} PRIVATE -fmodules)
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    # MSVC
    target_compile_options(${PROJECT_NAME} PRIVATE /std:c++20 /interface /experimental:module)
endif()