#include "NoiseMapGenerator.h"


/* init
 */
void NoiseMapGenerator::init() {
    PerlinNoise::init();
}

/* generate_noise_map
 */
void NoiseMapGenerator::generate_noise_map(NoiseMap &noise_map, float offset_x, float offset_y, NoiseMapSettings settings) {
    for (int x = 0; x < settings.width; x++) {
        for (int y = 0; y < settings.height; y++) {
            float sample_x = (float)(x + offset_x) / (float)settings.width * settings.scale;
            float sample_y = (float)(y + offset_y) / (float)settings.height * settings.scale;

            noise_map[glm::vec2(x, y)] = PerlinNoise::perlin_noise(sample_x, sample_y, settings.width, settings.height);
        }
    }
}