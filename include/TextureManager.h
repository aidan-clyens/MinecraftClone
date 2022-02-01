#pragma once

// Includes
#include "Engine/TextureCubeMap.h"
#include "BlockAtlas.h"

#include <vector>
#include <unordered_map>

// Defines
#define TEXTURE_WIDTH 16

// Enums
/* eBlockType
 */
typedef enum {
    BLOCK_GRASS,
    BLOCK_DIRT,
    BLOCK_STONE
} eBlockType;

/* eBlockAtlas
 */
typedef enum {
    GRASS_SIDE,
    GRASS_BOTTOM,
    GRASS_TOP,
    STONE
} eBlockAtlas;


// Typedefs
typedef std::unordered_map<eBlockType, TextureCubeMap> TextureMap;
typedef std::unordered_map<eBlockType, TextureCubeMap>::iterator TextureMapIterator;

/* TextureManager
 */
class TextureManager {
    public:
        static TextureManager *get_instance();

        void load_textures();

        TextureCubeMap get_texture(eBlockType type);

    private:
        TextureManager();

        static TextureManager *p_instance;

        TextureMap m_texture_map;
        BlockAtlas m_block_atlas;
};