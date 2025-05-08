cmake_minimum_required(VERSION 4.0.0)

set(CMAKE_EXPERIMENTAL_CXX_IMPORT_STD
        "a9e1cf81-9932-4810-974b-6eccaf14e457"
        CACHE STRING "" FORCE)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_MODULE_STD 1)

project(AnalogEngineerCalculator LANGUAGES C CXX)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake_modules")

find_package(glfw3 REQUIRED)
find_package(OpenGL REQUIRED)

include_directories(includes)

add_library(Logger)
target_sources(Logger PUBLIC
        FILE_SET CXX_MODULES FILES
        utils/Logger.cppm
)

add_library(RendererModule)
target_sources(RendererModule PUBLIC
FILE_SET CXX_MODULES FILES
        renderer/Window.cppm
        renderer/Renderer.cppm
)
target_link_libraries(RendererModule PUBLIC Logger GL glfw)


add_executable(AnalogEngineerCalculator main.cpp glad.c
)

target_link_libraries(AnalogEngineerCalculator RendererModule)

target_compile_options(AnalogEngineerCalculator PRIVATE
        -fmodules
        -Og
        -fno-omit-frame-pointer
        -march=native)


