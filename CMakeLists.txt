cmake_minimum_required(VERSION 3.20)
project(Demo LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# -----------------------------------------
# System-installed libraries
# -----------------------------------------

# SDL3 installed somewhere CMake can see it
find_package(SDL3 REQUIRED CONFIG)

# GLAD installed as a system package or local config
#   (via vcpkg, pacman, apt, brew, or your own install)
find_package(glad REQUIRED CONFIG)

# -----------------------------------------
# Fetch only Dear ImGui
# -----------------------------------------
include(FetchContent)

FetchContent_Declare(
    imgui
    GIT_REPOSITORY https://github.com/ocornut/imgui.git
    GIT_TAG v1.91.0
)
FetchContent_MakeAvailable(imgui)

# Build ImGui + backends as a small helper library
add_library(imgui_backend
    ${imgui_SOURCE_DIR}/backends/imgui_impl_sdl3.cpp
    ${imgui_SOURCE_DIR}/backends/imgui_impl_opengl3.cpp
)

target_include_directories(imgui_backend PUBLIC
    ${imgui_SOURCE_DIR}
    ${imgui_SOURCE_DIR}/backends
)

target_link_libraries(imgui_backend PUBLIC
    SDL3::SDL3
    glad::glad          # make sure you use your package's target name
)

# -----------------------------------------
# Your executable
# -----------------------------------------
add_executable(Demo
    main.cpp
    shaders.h
)

target_link_libraries(Demo PRIVATE
    SDL3::SDL3
    glad::glad
    imgui_backend
)


