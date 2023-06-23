set(ProjId opengl_debugger)

add_executable(${ProjId} main.cpp)
target_link_libraries(${ProjId} PRIVATE imgui opengl32 glfw3)
target_link_directories(${ProjId} PRIVATE ${CMAKE_SOURCE_DIR}/imgui/examples/libs/glfw/lib-vc2010-64)

set_target_properties(${ProjId} PROPERTIES
RUNTIME_OUTPUT_DIRECTORY ${Bin_Root}
ARCHIVE_OUTPUT_DIRECTORY ${Lib_Root}
)