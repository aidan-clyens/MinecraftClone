#include "Chunk.h"


/* Chunk
 */
Chunk::Chunk(vec2 position):
m_position(position)
{
    // Generate height map
    HeightMapSettings settings;
    settings.seed = HEIGHT_MAP_SEED;
    settings.width = CHUNK_WIDTH;
    settings.height = CHUNK_WIDTH;
    settings.scale = HEIGHT_MAP_SCALE;
    settings.depth = HEIGHT_MAP_DEPTH;
    settings.octaves = HEIGHT_MAP_OCTAVES;
    settings.persistence = HEIGHT_MAP_PERSISTENCE;
    settings.lacunarity = HEIGHT_MAP_LACUNARITY;

    HeightMap height_map;
    HeightMapGenerator::generate_height_map(height_map, m_position.x, m_position.y, settings);

    for (int x = 0; x < CHUNK_WIDTH; x++) {
        for (int z = 0; z < CHUNK_WIDTH; z++) {
            int max_height = CHUNK_DEPTH + (int)std::floor(height_map[vec2(x, z)]);

            for (int y = 0; y < max_height; y++) {
                eBlockType type;
                if (y == max_height - 1) {
                    type = BLOCK_GRASS;
                }
                else if (y < max_height - 1 && y > max_height - 5) {
                    type = BLOCK_DIRT;
                }
                else {
                    type = BLOCK_STONE;
                }

                m_blocks[vec3(x + m_position.x * (CHUNK_WIDTH - 1), y, z + m_position.y * (CHUNK_WIDTH - 1))] = type;
            }
        }
    }
}

/* get_block_map
 */
BlockTypeMap *Chunk::get_block_map() {
    return &m_blocks;
}