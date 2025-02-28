cmake_minimum_required(VERSION 3.23)
project(TreeGenerator)

# Web Configuration
if (EMSCRIPTEN)
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -s USE_GLFW=3 -s ASSERTIONS=1 -s WASM=1 -s ASYNCIFY -s GL_ENABLE_GET_PROC_ADDRESS=1")
    set(CMAKE_EXECUTABLE_SUFFIX ".html")
endif ()

set(CMAKE_CXX_STANDARD 23)

# Generate compile_commands.json
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Raylib
set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE) # don't build the supplied examples
set(BUILD_GAMES OFF CACHE BOOL "" FORCE) # or games
add_subdirectory(thirdparty/raylib)

# RayGUI (Assumes source is in thirdparty/raygui/src)
set(BUILD_RAYGUI_EXAMPLES OFF CACHE BOOL "" FORCE) # don't build raygui examples
add_subdirectory(thirdparty/raygui/projects/CMake)

# Project
add_executable(${PROJECT_NAME} src/main.cpp src/Trunk.cpp src/Trunk.h src/TreeNode.cpp src/TreeNode.h src/Random.cpp src/Random.h src/TopDownTree.cpp src/TopDownTree.h src/SideViewTree.cpp src/SideViewTree.h src/Canopy.cpp src/Canopy.h src/App.cpp src/App.h src/Tree.h)
target_link_libraries(${PROJECT_NAME} raylib raygui)

# Checks if OSX and links appropriate frameworks (Only required on MacOS)
if (APPLE)
    target_link_libraries(${PROJECT_NAME} "-framework IOKit")
    target_link_libraries(${PROJECT_NAME} "-framework Cocoa")
    target_link_libraries(${PROJECT_NAME} "-framework OpenGL")
endif ()
