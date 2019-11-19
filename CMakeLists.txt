cmake_minimum_required(VERSION 3.15)
project(opengl_raytracing)

set(CMAKE_CXX_STANDARD 14)

include_directories(third_party)
include_directories(third_party/imgui)

add_library(glad third_party/glad/glad.c)
target_link_libraries(glad ${CMAKE_DL_LIBS})

find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)

add_executable(opengl_raytracing main.cpp OpenGLDisplay.cpp OpenGLDisplay.h Config.cpp Config.h
        third_party/imgui/examples/imgui_impl_glfw.cpp
        third_party/imgui/examples/imgui_impl_opengl3.cpp
        third_party/imgui/imgui.cpp
        third_party/imgui/imgui_draw.cpp
        third_party/imgui/imgui_widgets.cpp
        third_party/imgui/imgui_demo.cpp AssetManager.cpp AssetManager.h MeshObject.cpp MeshObject.h Texture.cpp Texture.h Scene.cpp Scene.h Camera.cpp Camera.h Ray.cpp Ray.h)

target_include_directories(opengl_raytracing PUBLIC ${OPENGL_INCLUDE_DIR})
target_link_libraries(opengl_raytracing glad glfw ${OPENGL_gl_LIBRARY} ${OPENGL_glu_LIBRARY} png jsoncpp OpenMeshCore)