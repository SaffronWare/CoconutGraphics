cmake_minimum_required (VERSION 3.8)

project ("CoconutGraphics")


set(SRC_DIR ${CMAKE_SOURCE_DIR}/src)
set(LIB_DIR ${CMAKE_SOURCE_DIR}/libs)
set(SHDR_DIR ${SRC_DIR}/shaders)

file(GLOB SOURCES 
	"${SRC_DIR}/cpp/*.cpp" 
	"${SRC_DIR}/include/*.h" 
	"${LIB_DIR}/ImGui/*.cpp"
	"${LIB_DIR}/ImGui/*.h"
    "${LIB_DIR}/ImGui/backends/imgui_impl_glfw.cpp"
	"${LIB_DIR}/ImGui/backends/imgui_impl_opengl3.cpp"
	"${LIB_DIR}/ImGui/backends/imgui_impl_glfw.h"
	"${LIB_DIR}/ImGui/backends/imgui_impl_opengl3.h"
	

)

message(STATUS "Found source files: ${SOURCES}")


add_library(GLFW STATIC IMPORTED)
set_target_properties(GLFW PROPERTIES IMPORTED_LOCATION  "${LIB_DIR}/glfw-3.4/lib-vc-2022/glfw3.lib" )
target_include_directories(GLFW INTERFACE "${LIB_DIR}/glfw-3.4/include")

add_library(GLAD "${LIB_DIR}/OpenGL/src/glad.c")
target_include_directories(GLAD PUBLIC "${LIB_DIR}/OpenGL/include")


add_executable (CoconutGraphics ${SOURCES})
target_include_directories(CoconutGraphics PRIVATE
	"${SRC_DIR}/include"
    ${LIB_DIR}/ImGui             # Include ImGui headers
    ${LIB_DIR}/ImGui/backends    # Include backend headers
)

target_link_libraries(CoconutGraphics PUBLIC GLAD)
target_link_libraries(CoconutGraphics PRIVATE GLFW)


file(COPY "${LIB_DIR}/glfw-3.4/lib-vc-2022/glfw3.dll" DESTINATION ${CMAKE_BINARY_DIR})

if (SHDR_DIR)
	target_compile_definitions(CoconutGraphics PRIVATE SHDR_DIR=\"${SHDR_DIR}\")
endif()

if (SRC_DIR)
	target_compile_definitions(CoconutGraphics PRIVATE SRC_DIR=\"${SRC_DIR}\")
endif()

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET CoconutGraphics PROPERTY CXX_STANDARD 20)
endif()

