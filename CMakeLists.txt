cmake_minimum_required(VERSION 3.10)
project(AstrayOpenGL VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find required packages
find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)
find_package(box2d REQUIRED)

# Find SDL2_mixer
find_package(PkgConfig REQUIRED)
pkg_check_modules(SDL2 REQUIRED sdl2)
pkg_check_modules(SDL2_MIXER REQUIRED SDL2_mixer)

# Find GLM
find_path(GLM_INCLUDE_DIR glm/glm.hpp
    HINTS /usr/local/include /usr/include /opt/homebrew/include)

if(NOT GLM_INCLUDE_DIR)
    message(FATAL_ERROR "GLM not found. Please install GLM.")
endif()


include_directories(
    ${OPENGL_INCLUDE_DIRS}
    ${GLUT_INCLUDE_DIRS}
    ${GLM_INCLUDE_DIR}
    ${SDL2_INCLUDE_DIRS}
    ${SDL2_MIXER_INCLUDE_DIRS}
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/src
)

# Source files
set(SOURCES
    src/main.cpp
    src/game.cpp
    src/maze.cpp
    src/physics.cpp
    src/camera.cpp
    src/input.cpp
    src/audio.cpp
)

add_executable(main ${SOURCES})

link_directories(${SDL2_LIBRARY_DIRS} ${SDL2_MIXER_LIBRARY_DIRS})

target_link_libraries(main
    ${OPENGL_LIBRARIES}
    ${GLUT_LIBRARIES}
    box2d::box2d
    "-L/opt/homebrew/lib"
    "-lSDL2"
    "-lSDL2_mixer"
)


file(COPY ${CMAKE_SOURCE_DIR}/assets DESTINATION ${CMAKE_BINARY_DIR})

if(APPLE)
    target_link_libraries(main "-framework Cocoa" "-framework IOKit" "-framework CoreVideo")
endif()

set_target_properties(main PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}
)
