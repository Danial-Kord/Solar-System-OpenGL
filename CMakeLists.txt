cmake_minimum_required(VERSION 3.15.0)
project(453-skeleton)

set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/modules/" ${CMAKE_MODULE_PATH})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

# Use modern C++
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(OpenGL_GL_PREFERENCE GLVND)

#-------------------------------------------------------------------------------
# https://github.com/adishavit/argh/releases/tag/v1.3.1
include_directories(SYSTEM thirdparty/argh-1.3.1/)

#-------------------------------------------------------------------------------
# https://glad.dav1d.de/
add_subdirectory(thirdparty/glad)
set(LIBRARIES ${LIBRARIES} glad)

#-------------------------------------------------------------------------------
# http://glew.sourceforge.net/
include_directories(SYSTEM thirdparty/glew-2.1.0/include)

#-------------------------------------------------------------------------------
# https://www.glfw.org/

# Turn off building their docs/tests/examples.
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

add_subdirectory(thirdparty/glfw-3.3.2)
set(LIBRARIES ${LIBRARIES} glfw)

#-------------------------------------------------------------------------------
# https://github.com/gurki/vivid/releases/tag/v2.2.1
include_directories(SYSTEM thirdparty/vivid-2.2.1/include)
include_directories(SYSTEM thirdparty/vivid-2.2.1/dependencies/glm)

#-------------------------------------------------------------------------------
# https://glm.g-truc.net/0.9.9/index.html
include_directories(SYSTEM thirdparty/glm-0.9.9.7/)

#-------------------------------------------------------------------------------
# https://fmt.dev/latest/index.html
# Turn off building their tests
set(FMT_TES OFF CACHE BOOL "" FORCE)
add_subdirectory(thirdparty/fmt-7.0.3)
set(LIBRARIES ${LIBRARIES} fmt::fmt)
include_directories(SYSTEM thirdparty/fmt-7.0.3/include)

include_directories(SYSTEM thirdparty/stb-2.26)
include_directories(SYSTEM thirdparty/imgui-1.78)

find_package(OpenGL REQUIRED)
set(LIBRARIES ${LIBRARIES} ${OPENGL_gl_LIBRARY})


if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
	list(APPEND _453_CMAKE_CXX_FLAGS ${_453_CMAKE_CXX_FLAGS} "-Wall" "-pedantic")
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
	# using GCC
	list(APPEND _453_CMAKE_CXX_FLAGS ${_453_CMAKE_CXX_FLAGS} "-fdiagnostics-color=always" "-Wall" "-pedantic")
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Intel")
	# using Intel C++
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
	add_compile_options("/permissive-")
	add_compile_options("/Zc:preprocessor")
	# Remove the default warning level.
	# https://stackoverflow.com/questions/58708772/cmake-project-in-visual-studio-gives-flag-override-warnings-command-line-warnin
	string(REGEX REPLACE "/W[3|4]" "" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
	 # The above preprocessor warns about certain things in windows headers.
	 # Turn those off: https://docs.microsoft.com/en-us/cpp/error-messages/compiler-warnings/c5105?view=vs-2019
	add_compile_options("/wd5105")

	set(_453_CMAKE_CXX_FLAGS ${_453_CMAKE_CXX_FLAGS} /W3)

endif()

if(APPLE)
	set(LIBRARIES ${LIBRARIES} pthread dl)
elseif(UNIX)
	set(LIBRARIES ${LIBRARIES} pthread GL dl)
elseif(WIN32)
endif()


# add_definitions(-DIMGUI_IMPL_OPENGL_LOADER_GLAD=ON)
# include_directories(SYSTEM thirdparty/imgui thirdparty/imgui/examples)
# include_directories(src)


# Compile our main application
file(GLOB SOURCES
    453-skeleton/*
    thirdparty/glew-2.1.0/src/glew.c
	thirdparty/imgui-1.78/imgui/*.cpp
)
set(INCLUDES ${INCLUDES} src)

set(APP_NAME "453-skeleton")


# Copy all the shaders and tell the build system to re-run CMAKE if one of them changes
file(GLOB files 453-skeleton/shaders/*)
foreach(file ${files})
	get_filename_component(name ${file} NAME)
	configure_file(${file} shaders/${name})
endforeach()

add_executable(${APP_NAME} ${SOURCES} "453-skeleton/SimpleCamera.h"  "453-skeleton/Planet.h" "453-skeleton/Geometry.h")

include_directories("images")
configure_file("images/earth2k.jpg" "images/earth2k.jpg" COPYONLY)
configure_file("images/2k_moon.jpg" "images/2k_moon.jpg" COPYONLY)
configure_file("images/2k_sun.jpg" "images/2k_sun.jpg" COPYONLY)
	

target_include_directories(${APP_NAME} PRIVATE ${INCLUDES})
target_link_libraries(${APP_NAME} ${LIBRARIES})
target_compile_definitions(${APP_NAME} PRIVATE ${DEFINITIONS})
target_compile_options(${APP_NAME} PRIVATE ${_453_CMAKE_CXX_FLAGS})
set_target_properties(${APP_NAME} PROPERTIES INSTALL_RPATH "./" BUILD_RPATH "./")
