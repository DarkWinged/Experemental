cmake_minimum_required(VERSION 3.8)
project(Experimental)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES main.cpp inputController.cpp inputController.h physicsObject.cpp physicsObject.h physicsMath.cpp physicsMath.h)
add_executable(Experimental ${SOURCE_FILES} inputController.cpp inputController.h physicsObject.cpp physicsObject.h)

INCLUDE(FindPkgConfig)

PKG_SEARCH_MODULE(SDL2 REQUIRED sdl2)
PKG_SEARCH_MODULE(SDL2IMAGE REQUIRED SDL2_image>=2.0.0)

INCLUDE_DIRECTORIES(${SDL2_INCLUDE_DIRS} ${SDL2IMAGE_INCLUDE_DIRS})
TARGET_LINK_LIBRARIES(${PROJECT_NAME} ${SDL2_LIBRARIES} ${SDL2IMAGE_LIBRARIES})