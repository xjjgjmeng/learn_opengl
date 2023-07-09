cmake_minimum_required (VERSION 3.0)
cmake_policy(VERSION 3.0)

set(Bin_Root ${CMAKE_SOURCE_DIR}/Bin)
set(Lib_Root ${CMAKE_SOURCE_DIR}/Lib)

project(learn_opengl)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)

# glad
include_directories(${CMAKE_SOURCE_DIR}/third_party/includes)
add_library(GLAD "${CMAKE_SOURCE_DIR}/third_party/glad.c")

add_library(imgui STATIC
${CMAKE_SOURCE_DIR}/imgui/imgui.cpp
${CMAKE_SOURCE_DIR}/imgui/imgui_demo.cpp
${CMAKE_SOURCE_DIR}/imgui/imgui_draw.cpp
${CMAKE_SOURCE_DIR}/imgui/imgui_widgets.cpp
${CMAKE_SOURCE_DIR}/imgui/imgui_tables.cpp
${CMAKE_SOURCE_DIR}/imgui/backends/imgui_impl_glfw.cpp
${CMAKE_SOURCE_DIR}/imgui/backends/imgui_impl_opengl3.cpp
)
target_include_directories(imgui PUBLIC
${CMAKE_SOURCE_DIR}/imgui
${CMAKE_SOURCE_DIR}/imgui/examples/libs/glfw/include
${CMAKE_SOURCE_DIR}/imgui/backends
)
set_target_properties(imgui PROPERTIES
RUNTIME_OUTPUT_DIRECTORY ${Bin_Root}
ARCHIVE_OUTPUT_DIRECTORY ${Lib_Root}
)

add_subdirectory(src)