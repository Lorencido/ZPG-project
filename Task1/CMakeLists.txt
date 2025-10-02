cmake_minimum_required(VERSION 4.0)
project(untitled)

set(CMAKE_CXX_STANDARD 20)

if (CMAKE_CXX_COMPILER_ID STREQUAL "AppleClang")
    execute_process(
            COMMAND xcrun --show-sdk-path
            OUTPUT_VARIABLE SDK_PATH
            OUTPUT_STRIP_TRAILING_WHITESPACE
    )
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++ -isysroot ${SDK_PATH} -I${SDK_PATH}/usr/include/c++/v1")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -stdlib=libc++ -L${SDK_PATH}/usr/lib")
endif()

set(CMAKE_PREFIX_PATH "/opt/homebrew/opt/glfw;/opt/homebrew/opt/glm;/opt/homebrew/opt/glew")
find_package(glfw3 3.4 REQUIRED)
find_package(glm REQUIRED)
find_package(GLEW REQUIRED)

add_executable(untitled main.cpp)

target_link_libraries(untitled PRIVATE
        glfw
        glm::glm
        GLEW::GLEW
        "-framework Cocoa"
        "-framework IOKit"
        "-framework CoreVideo"
        "-framework OpenGL")

