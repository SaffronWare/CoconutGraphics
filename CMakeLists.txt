cmake_minimum_required (VERSION 3.8)

project ("CoconutGraphics")


set(SRC_DIR ${CMAKE_SOURCE_DIR}/src)
set(LIB_DIR ${CMAKE_SOURCE_DIR}/libs)
set(ASSET_DIR ${CMAKE_SOURCE_DIR}/assets)
set(SHDR_DIR ${SRC_DIR}/shaders)
set(ASSET_DIR ${CMAKE_SOURCE_DIR}/assets)

file(GLOB SOURCES 
	"${SRC_DIR}/cpp/*.cpp" 
	"${SRC_DIR}/include/*.h" 
	"${LIB_DIR}/ImGui/*.cpp"
	"${LIB_DIR}/ImGui/*.h"
	"${LIB_DIR}/ImGui/backends/*.h"
	"${LIB_DIR}/ImGui/backends/*.cpp"
	"${LIB_DIR}/STB_IMAGE/stb_image.h"
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
    "${LIB_DIR}/ImGui"         
    "${LIB_DIR}/ImGui/backends"
	"${LIB_DIR}/STB_IMAGE"
)

target_link_libraries(CoconutGraphics PUBLIC GLAD)
target_link_libraries(CoconutGraphics PRIVATE GLFW)


file(COPY "${LIB_DIR}/glfw-3.4/lib-vc-2022/glfw3.dll" DESTINATION ${CMAKE_BINARY_DIR})
file(COPY ${SHDR_DIR} DESTINATION ${CMAKE_BINARY_DIR})
file(COPY ${ASSET_DIR} DESTINATION ${CMAKE_BINARY_DIR})
file(COPY "${SRC_DIR}/imgui.ini" DESTINATION ${CMAKE_BINARY_DIR})



if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET CoconutGraphics PROPERTY CXX_STANDARD 20)
endif()

