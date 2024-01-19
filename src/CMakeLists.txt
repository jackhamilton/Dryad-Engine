cmake_minimum_required(VERSION 3.18)

project(
  dryad
  VERSION "0.1.0"
  DESCRIPTION "A custom 3D game engine that utilizes the Entity-Component System paradigm"
  LANGUAGES CXX)

#set_property(GLOBAL PROPERTY CMAKE_CXX_STANDARD 20)

include(FetchContent)
include(GenerateExportHeader)
#find_package(SDL_ttf REQUIRED)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(BUILD_UNIT_TESTS OFF CACHE BOOL "" FORCE)
set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)
set(BUILD_CPU_DEMOS OFF CACHE BOOL "" FORCE)
set(BUILD_BULLET2_DEMOS OFF CACHE BOOL "" FORCE)
set(BUILD_EXTRAS OFF CACHE BOOL "" FORCE)
set(BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(ALSOFT_INSTALL OFF CACHE BOOL "" FORCE)
set(ALSOFT_INSTALL_HRTF_DATA OFF CACHE BOOL "" FORCE)
set(ALSOFT_INSTALL_AMBDEC_PRESETS OFF CACHE BOOL "" FORCE)
set(ALSOFT_INSTALL_EXAMPLES OFF CACHE BOOL "" FORCE)
set(ALSOFT_INSTALL_UTILS OFF CACHE BOOL "" FORCE)
set(BUILD_PROGRAMS OFF CACHE BOOL "" FORCE)
set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(LIBTYPE STATIC CACHE STRING "" FORCE)

FetchContent_Declare(
  sdl2
  GIT_REPOSITORY https://github.com/libsdl-org/SDL
  GIT_TAG release-2.0.22)

FetchContent_Declare(
  sdl2_ttf
  GIT_REPOSITORY https://github.com/libsdl-org/SDL_ttf
  GIT_TAG release-2.20.1)

FetchContent_MakeAvailable(sdl2 sdl2_ttf)

include_directories(${SDL2TTF_INCLUDE_DIRS})

add_subdirectory("dryad")
add_subdirectory("samples")
