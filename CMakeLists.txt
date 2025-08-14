cmake_minimum_required(VERSION 3.16)
project(Lite2DEngine LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(NOT CMAKE_BUILD_TYPE)
	set(CMAKE_BUILD_TYPE Release)
endif()

add_compile_options(
	$<$<CXX_COMPILER_ID:GNU,Clang>:-Wall>
	$<$<CXX_COMPILER_ID:GNU,Clang>:-Wextra>
	$<$<CXX_COMPILER_ID:GNU,Clang>:-Wpedantic>
)

find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)

file(GLOB_RECURSE ENGINE_HEADERS CONFIGURE_DEPENDS include/*.hpp)
file(GLOB_RECURSE ENGINE_SOURCES CONFIGURE_DEPENDS src/*.cpp)

add_executable(Lite2DEngine
	${ENGINE_HEADERS}
	${ENGINE_SOURCES}
)

target_include_directories(Lite2DEngine PRIVATE include)

target_link_libraries(Lite2DEngine PRIVATE SDL2::SDL2 SDL2_image::SDL2_image)

# On Windows, ensure a console subsystem and correct runtime
if(WIN32)
	target_link_libraries(Lite2DEngine PRIVATE SDL2::SDL2main)
endif()