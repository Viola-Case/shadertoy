set(CMAKE_TOOLCHAIN_FILE "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake"
    CACHE STRING "Vcpkg toolchain file")
set(VCPKG_MANIFEST_MODE ON)

cmake_minimum_required(VERSION 4.2)

project(ShaderToy VERSION 1.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/bin)

file(GLOB_RECURSE SHADERTOY_SOURCES "src/*.cpp" "src/*.h")
add_executable(ShaderToy ${SHADERTOY_SOURCES})

find_package(SDL3 REQUIRED)
find_package(imgui REQUIRED)
find_package(glad REQUIRED)

target_link_libraries(ShaderToy
    PRIVATE SDL3::SDL3
    PRIVATE imgui::imgui
    PRIVATE glad::glad
)
