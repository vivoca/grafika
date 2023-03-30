cmake_minimum_required(VERSION 3.24)
project(grafika)

add_executable(grafika main.cpp)

set(CMAKE_CXX_STANDARD 17)

find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)
include_directories( ${OPENGL_INCLUDE_DIRS}  ${GLUT_INCLUDE_DIRS} )

target_link_libraries(grafika "-framework GLUT")
target_link_libraries(grafika "-framework Cocoa")
set_target_properties(grafika PROPERTIES LINK_FLAGS "-Wl,-F/Library/Frameworks")

target_link_libraries(grafika ${OPENGL_LIBRARIES} ${GLUT_LIBRARY} )
