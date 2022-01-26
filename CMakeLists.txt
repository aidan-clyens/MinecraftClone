cmake_minimum_required(VERSION 3.0)

project(MinecraftChallenge VERSION 3.0)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_subdirectory(lib/3DEngine)

add_executable(${PROJECT_NAME} src/main.cpp src/PerlinNoise.cpp)
target_include_directories(${PROJECT_NAME} PUBLIC include)
target_include_directories(${PROJECT_NAME} PUBLIC lib/3DEngine/include)
target_link_libraries(${PROJECT_NAME} EngineCore)
