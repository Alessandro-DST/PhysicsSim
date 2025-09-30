cmake_minimum_required(VERSION 3.10)

project(PhysicsSim VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Eigen3 REQUIRED)

include_directories(include)

file(GLOB SOURCES src/*.cpp external/glad/src/*.c)

add_executable(PhysicsSim ${SOURCES})

target_include_directories(PhysicsSim PRIVATE
    external/glad/include
)

find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)

target_link_libraries(PhysicsSim PRIVATE Eigen3::Eigen glfw OpenGL::GL)


