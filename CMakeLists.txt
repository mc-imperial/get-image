cmake_minimum_required(VERSION 2.8)

project(cpp)

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/Modules/" ${CMAKE_MODULE_PATH})

find_package(OpenGL REQUIRED)

include(ExternalProject)
INCLUDE(GNUInstallDirs)

if(APPLE)
	find_package(OpenGL REQUIRED)
	find_package(GLUT REQUIRED)
	set(
	LINK_LIBS 
	${OPENGL_LIBRARIES}
	${GLUT_LIBRARIES})
else()
ExternalProject_Add(
	GLUT
	URL http://prdownloads.sourceforge.net/freeglut/freeglut-3.0.0.tar.gz
	CMAKE_ARGS 
		-DCMAKE_TOOLCHAIN_FILE=${CMAKE_TOOLCHAIN_FILE}
		-DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
		-DFREEGLUT_REPLACE_GLUT=0
		-DFREEGLUT_BUILD_DEMOS=0
	INSTALL_COMMAND cmake -DCMAKE_INSTALL_PREFIX=${CMAKE_BINARY_DIR}/install -P cmake_install.cmake
	)
endif()


ExternalProject_Add(
	GLEW
	URL https://sourceforge.net/projects/glew/files/glew/2.0.0/glew-2.0.0.tgz
	CMAKE_ARGS
		../GLEW/build/cmake/
		-DCMAKE_TOOLCHAIN_FILE=${CMAKE_TOOLCHAIN_FILE}
		-DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
	INSTALL_COMMAND cmake -DCMAKE_INSTALL_PREFIX=${CMAKE_BINARY_DIR}/install -P cmake_install.cmake
	)


set(CMAKE_PREFIX_PATH ${CMAKE_BINARY_DIR}/install ${CMAKE_PREFIX_PATH})


if(MINGW)
	set(
	LINK_LIBS 
	"${CMAKE_BINARY_DIR}/install/lib/libfreeglut_static.a" 
	"${CMAKE_BINARY_DIR}/install/lib/libglew32.a"
	winmm.a
	${OPENGL_gl_LIBRARY})

elseif(WIN32)
	set(
	LINK_LIBS 
	"${CMAKE_BINARY_DIR}/install/lib/libfreeglut_static.a" 
	"${CMAKE_BINARY_DIR}/install/lib/libglew32.a"
	${OPENGL_gl_LIBRARY})

elseif(UNIX)

	if(APPLE)
	else()
		find_package(X11 REQUIRED)
		set(
		LINK_LIBS 
		"${CMAKE_BINARY_DIR}/install/${CMAKE_INSTALL_LIBDIR}/libfreeglut.a" 
		"${CMAKE_BINARY_DIR}/install/${CMAKE_INSTALL_LIBDIR}/libGLEW.a"
		${OPENGL_gl_LIBRARY}
		${X11_LIBRARIES}
		${X11_Xxf86vm_LIB}
		${X11_Xi_LIB}
		${X11_Xrandr_LIB})
	endif()
	
endif()


add_definitions(-DGLEW_STATIC -DFREEGLUT_STATIC)


set(INCLUDE_DIRS ${OPENGL_INCLUDE_DIR} ${CMAKE_BINARY_DIR}/install/include)

add_executable(get_image get_image.cpp lodepng.cpp)
add_executable(get_gl_info get_gl_info.cpp)

if(CMAKE_COMPILER_IS_GNUCXX)
	set_target_properties(get_image get_gl_info PROPERTIES LINK_SEARCH_START_STATIC 1 LINK_SEARCH_END_STATIC 1)
	set(CMAKE_CXX_FLAGS "-Wall -std=c++11 ${CMAKE_CXX_FLAGS}")
	set(CMAKE_EXE_LINKER_FLAGS "-static-libgcc -static-libstdc++ -static -Wl,-Bstatic")
endif()

target_include_directories(get_image PRIVATE ${INCLUDE_DIRS})
target_link_libraries(get_image ${LINK_LIBS})

target_include_directories(get_gl_info PRIVATE ${INCLUDE_DIRS})
target_link_libraries(get_gl_info ${LINK_LIBS})

install(TARGETS get_image get_gl_info
	DESTINATION bin
)
