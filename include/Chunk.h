#pragma once

// Includes
#include "Engine/utils/types.h"

#include "Terrain/HeightMapGenerator.h"
#include "TextureManager.h"

#include <vector>

// Defines
#define CHUNK_WIDTH                 16
#define CHUNK_DEPTH                 64

#define HEIGHT_MAP_SEED             123456
#define HEIGHT_MAP_SCALE            1
#define HEIGHT_MAP_DEPTH            5
#define HEIGHT_MAP_OCTAVES          3
#define HEIGHT_MAP_PERSISTENCE      0.2
#define HEIGHT_MAP_LACUNARITY       2

// Typedefs
typedef std::unordered_map<vec3, eBlockType, vec3_key_hash, vec3_key_equal> BlockTypeMap;
typedef std::unordered_map<vec3, eBlockType, vec3_key_hash, vec3_key_equal>::iterator BlockTypeMapIterator;

/* Chunk
 */
class Chunk {
    public:
        Chunk(vec2 position);

        BlockTypeMap *get_block_map();
    
    private:
        vec2 m_position;
        BlockTypeMap m_blocks;
};