
project(cpp)

find_package(OpenGL REQUIRED)

if(WIN32)
	# hacks required to ensure static libraries are found first
	set(CMAKE_FIND_LIBRARY_PREFIXES "lib" "")
	set(CMAKE_FIND_LIBRARY_SUFFIXES "_static.lib" "s.lib" ".lib")
	
	add_definitions(-DGLEW_STATIC -DFREEGLUT_STATIC)
else()
	set (CMAKE_CXX_FLAGS "-Wall -std=c++11 ${CMAKE_CXX_FLAGS}")
endif()

find_package(GLUT REQUIRED)
find_package(GLEW REQUIRED)

add_executable(get_image get_image.cpp lodepng.cpp)
add_executable(get_gl_info get_gl_info.cpp)

set(INCLUDE_DIRS ${OPENGL_INCLUDE_DIR} ${GLEW_INCLUDE_DIRS} ${GLUT_INCLUDE_DIR})
set(LINK_DIRS ${OPENGL_gl_LIBRARY} ${GLUT_glut_LIBRARY} ${GLEW_LIBRARY})

target_include_directories(get_image PRIVATE ${INCLUDE_DIRS})
target_link_libraries(get_image ${LINK_DIRS})

target_include_directories(get_gl_info PRIVATE ${INCLUDE_DIRS})
target_link_libraries(get_gl_info ${LINK_DIRS})

install(TARGETS get_image get_gl_info
	DESTINATION bin
)
