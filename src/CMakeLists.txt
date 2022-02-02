project(Game)

add_subdirectory(Terrain)

add_library(${PROJECT_NAME} Game.cpp Block.cpp Chunk.cpp World.cpp BlockAtlas.cpp TextureManager.cpp ShaderManager.cpp)
target_include_directories(${PROJECT_NAME} PUBLIC ../include)
target_include_directories(${PROJECT_NAME} PUBLIC ../lib/3DEngine/include)
target_link_libraries(${PROJECT_NAME} EngineCore Terrain)
