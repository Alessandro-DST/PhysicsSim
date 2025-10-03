cmake_minimum_required(VERSION 3.15)

project(PhysicsSim VERSION 0.1 LANGUAGES CXX C)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Packages
find_package(Eigen3 REQUIRED)
find_package(OpenGL REQUIRED)
find_package(PkgConfig REQUIRED)

# Use pkg-config to find glfw3
pkg_check_modules(GLFW REQUIRED glfw3)

# Sources
file(GLOB SOURCES src/*.cpp)

# Glad library 
add_library(glad STATIC external/glad/glad/src/glad.c)
target_include_directories(glad PUBLIC external/glad/glad/include)
set_target_properties(glad PROPERTIES LINKER_LANGUAGE C)

# Executable
add_executable(PhysicsSim ${SOURCES})
target_include_directories(PhysicsSim PRIVATE
    include
    ${GLFW_INCLUDE_DIRS}   # Include GLFW headers from pkg-config
)

# Link libraries
target_link_libraries(PhysicsSim
    PRIVATE
    Eigen3::Eigen
    glad
    OpenGL::GL
    ${GLFW_LIBRARIES}      # Link GLFW libraries from pkg-config
)

# Compiler flags from pkg-config
target_compile_options(PhysicsSim PRIVATE ${GLFW_CFLAGS_OTHER})
