set(ProjId opengl_debugger)

file(GLOB my_file "*.h" "*.cpp" "*.fs" "*.vs")
add_executable(${ProjId} ${my_file})
target_link_libraries(${ProjId} PRIVATE imgui GLAD opengl32 glfw3)
target_link_directories(${ProjId} PRIVATE ${CMAKE_SOURCE_DIR}/imgui/examples/libs/glfw/lib-vc2010-64)
set_target_properties(${ProjId} PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})

set_target_properties(${ProjId} PROPERTIES
RUNTIME_OUTPUT_DIRECTORY ${Bin_Root}
ARCHIVE_OUTPUT_DIRECTORY ${Lib_Root}
)