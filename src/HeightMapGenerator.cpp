#include "HeightMapGenerator.h"


/* init
 */
void HeightMapGenerator::init() {
    NoiseMapGenerator::init();
}

/* generate_height_map
 */
void HeightMapGenerator::generate_height_map(HeightMap &height_map, int x, int y, HeightMapSettings settings) {
    // Generate noise map
    NoiseMap noise_map;

    int offset_x = (int)(x * (settings.width - 1)) + settings.seed;
    int offset_y = (int)(y * (settings.height - 1)) + settings.seed;

    NoiseMapSettings noise_settings;
    noise_settings.width = settings.width;
    noise_settings.height = settings.height;
    noise_settings.scale = settings.scale;
    noise_settings.octaves = settings.octaves;
    noise_settings.persistence = settings.persistence;
    noise_settings.lacunarity = settings.lacunarity;

    NoiseMapGenerator::generate_noise_map(noise_map, offset_x, offset_y, noise_settings);

    // Determine map depth
    for (int x = 0; x < settings.width; x++) {
        for (int y = 0; y < settings.width; y++) {
            height_map[glm::vec2(x, y)] = noise_map[glm::vec2(x, y)] * settings.depth;
        }
    }
}