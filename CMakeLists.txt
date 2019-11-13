cmake_minimum_required(VERSION 3.15)
project(opengl_raytracing)

set(CMAKE_CXX_STANDARD 14)

add_library(glad glad.c)
target_link_libraries(glad ${CMAKE_DL_LIBS})

find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)

add_executable(opengl_raytracing main.cpp OpenGLDisplay.cpp OpenGLDisplay.h Config.cpp Config.h)

target_include_directories(opengl_raytracing PUBLIC ${OPENGL_INCLUDE_DIR})
target_link_libraries(opengl_raytracing glad glfw ${OPENGL_gl_LIBRARY} ${OPENGL_glu_LIBRARY})