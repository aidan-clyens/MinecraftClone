#pragma once

// Includes
#include "Terrain/NoiseMapGenerator.h"

#include <unordered_map>


/* HeightMapSettings
 */
typedef struct {
    int seed;
    int width;
    int height;
    float scale;
    float depth;
    int octaves;
    float persistence;
    float lacunarity;
} HeightMapSettings;

// Typedefs
typedef std::unordered_map<glm::vec2, float, key_hash, key_equal> HeightMap;


/* HeightMapGenerator
 */
class HeightMapGenerator {
    public:
        static void init();
        static void generate_height_map(HeightMap &height_map, int x, int y, HeightMapSettings settings);
};