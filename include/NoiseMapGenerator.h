#pragma once

// Includes
#include "Engine/Object3D.h"

#include "PerlinNoise.h"

#include <unordered_map>


// Structs
/* key_hash
 */
struct key_hash : public std::unary_function<glm::vec2, std::size_t> {
    std::size_t operator() (const glm::vec2 &c) const {
        int x = (int)c.x;
        int y = (int)c.y;
        return x ^ y;
    }
};

/* key_equal
 */
struct key_equal : public std::binary_function<glm::vec2, glm::vec2, bool> {
    bool operator() (const glm::vec2 &c0, const glm::vec2 &c1) const {
        return c0 == c1;
    }
};

/* NoiseMapSettings
 */
typedef struct {
    int width;
    int height;
    float scale;
    int octaves;
    float persistence;
    float lacunarity;
} NoiseMapSettings;

// Typedefs
typedef std::unordered_map<glm::vec2, float, key_hash, key_equal> NoiseMap;


/* NoiseMapGenerator
 */
class NoiseMapGenerator {
    public:
        static void init();
        static void generate_noise_map(NoiseMap &noise_map, float offset_x, float offset_y, NoiseMapSettings settings);
};