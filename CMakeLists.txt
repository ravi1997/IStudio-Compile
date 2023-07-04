cmake_minimum_required(VERSION 3.22.1)

PROJECT(IStudio)
set(CMAKE_CXX_STANDARD 23)
#set(CMAKE_BUILD_TYPE "Debug")
add_compile_options(-g -Wall -Wextra -Wpedantic -Werror -Wno-subobject-linkage -O0)
add_subdirectory(src)
add_subdirectory(include)
add_executable(${PROJECT_NAME} ${SRC_FILES})

target_include_directories(${PROJECT_NAME} PRIVATE ${INC_DIRS})

install(TARGETS ${PROJECT_NAME}
        DESTINATION /usr/local/bin)