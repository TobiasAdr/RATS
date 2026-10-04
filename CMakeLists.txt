cmake_minimum_required(VERSION 3.16)
project(RATS!)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Set output directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Find required packages
find_package(SFML 3 COMPONENTS Graphics Window System CONFIG REQUIRED)
find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
find_package(glm REQUIRED)

# Include directories
include_directories(${CMAKE_SOURCE_DIR}/include)
include_directories(${OPENGL_INCLUDE_DIR})
include_directories(${GLEW_INCLUDE_DIRS})

# Source files - Main game
set(GAME_SOURCES
    src/main.cpp
    src/Game.cpp
    src/Character.cpp
    src/Player.cpp
    src/Enemy.cpp
    src/Platform.cpp
    src/EntityManager.cpp
    src/CollisionHandler.cpp
    src/InputHandler.cpp
    src/SpriteHandler.cpp
    src/weaponHandler.cpp
    src/World.cpp
    src/GameRenderer.cpp
    src/LightManager.cpp
)

# Source files - OpenGL
set(OPENGL_SOURCES
    src/OpenGL/OpenGLElementSimulation.cpp
    src/OpenGL/OpenGLWaterSimulation.cpp
    src/OpenGL/OpenGLFireSimulation.cpp
    src/OpenGL/OpenGLBouncingRods.cpp
    src/OpenGL/EnemyManagerOpenGL.cpp
    src/OpenGL/TileRendererOpenGL.cpp
    src/OpenGL/OpenGLRenderer.cpp
    src/OpenGL/spriteRenderer.cpp
    src/OpenGL/firePlaceOpenGL.cpp
)

# Combine all sources
set(ALL_SOURCES ${GAME_SOURCES} ${OPENGL_SOURCES})

# Create executable
add_executable(rats_game ${ALL_SOURCES})
set_target_properties(rats_game PROPERTIES
    OUTPUT_NAME "RATS!"
)

# Link libraries
target_link_libraries(rats_game
    PRIVATE
    SFML::Graphics SFML::Window SFML::System
    OpenGL::GL
    GLEW::GLEW
    glm::glm
)

# Platform specific settings
if(WIN32)
    set_target_properties(rats_game PROPERTIES
        WIN32_EXECUTABLE OFF
    )
endif()

# Copy assets to build directory
add_custom_command(TARGET rats_game POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/shaders $<TARGET_FILE_DIR:rats_game>/shaders
    COMMENT "Copying shaders to build directory"
)

add_custom_command(TARGET rats_game POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/resources $<TARGET_FILE_DIR:rats_game>/resources
    COMMENT "Copying resources to build directory"
)

# Print build info
message(STATUS "RATS! configured successfully!")
message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
message(STATUS "Source dir: ${CMAKE_SOURCE_DIR}")
message(STATUS "Binary dir: ${CMAKE_BINARY_DIR}")