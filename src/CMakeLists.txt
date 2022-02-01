project(Game)

add_library(${PROJECT_NAME} PerlinNoise.cpp NoiseMapGenerator.cpp HeightMapGenerator.cpp BlockAtlas.cpp TextureManager.cpp ShaderManager.cpp)
target_include_directories(${PROJECT_NAME} PUBLIC ../include)
target_include_directories(${PROJECT_NAME} PUBLIC ../lib/3DEngine/include)
target_link_libraries(${PROJECT_NAME} EngineCore)
