cmake_minimum_required(VERSION 3.17)
project(Golf_Game)

set(CMAKE_CXX_STANDARD 14)

set(SDL2_INCLUDE_DIR C:/Users/Aaron/libs/SDL/include)
set(SDL2_LIB_DIR C:/Users/Aaron/libs/SDL/lib/x64)

set(SDLttf_INCLUDE_DIR C:/Users/Aaron/libs/SDL2_ttf/include)
set(SDLttf_LIB_DIR C:/Users/Aaron/libs/SDL2_ttf/lib/x64)

include_directories(${SDL2_INCLUDE_DIR})
link_directories(${SDL2_LIB_DIR})

include_directories(${SDLttf_INCLUDE_DIR})
link_directories(${SDLttf_LIB_DIR})

add_executable(Golf_Game main.cpp SDL2_framerate.c SDL2_gfxPrimitives.c SDL2_imageFilter.c SDL2_rotozoom.c)
target_link_libraries(${PROJECT_NAME} SDL2main SDL2 SDL2_ttf)