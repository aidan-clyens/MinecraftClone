#include "Terrain/NoiseMapGenerator.h"


/* init
 */
void NoiseMapGenerator::init() {
    PerlinNoise::init();
}

/* generate_noise_map
 */
void NoiseMapGenerator::generate_noise_map(NoiseMap &noise_map, float offset_x, float offset_y, NoiseMapSettings settings) {
    float max_possible_height = 0;
    float amplitude = 1;
    float frequency = 1;

    // Determine max possible height to normalize noise map
    for (int i = 0; i < settings.octaves; i++) {
        max_possible_height += amplitude;
        amplitude *= settings.persistence;
    }

    for (int x = 0; x < settings.width; x++) {
        for (int y = 0; y < settings.height; y++) {
            float noise_height = 0;
            amplitude = 1;
            frequency = 1;

            for (int i = 0; i < settings.octaves; i++) {
                float sample_x = (float)(x + offset_x) / (float)settings.width * settings.scale * frequency;
                float sample_y = (float)(y + offset_y) / (float)settings.height * settings.scale * frequency;

                noise_height += PerlinNoise::perlin_noise(sample_x, sample_y, settings.width, settings.height) * amplitude;

                amplitude *= settings.persistence;
                frequency *= settings.lacunarity;
            }

            noise_map[vec2(x, y)] = noise_height;
        }
    }

    // Normalize noise map
    for (int x = 0; x < settings.width; x++) {
        for (int y = 0; y < settings.height; y++) {
            noise_map[vec2(x, y)] = NoiseMapGenerator::inverse_lerp(-max_possible_height, max_possible_height, noise_map[vec2(x, y)]);
        }
    }
}

/* inverse_lerp
 */
float NoiseMapGenerator::inverse_lerp(float a, float b, float value) {
    return (value - a) / (b - a);
}