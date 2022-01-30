#pragma once

// Includes
#include <glm/glm.hpp>

// Defines
#define PERMUTATION_SIZE 256


/* PerlinNoise
 */
class PerlinNoise {
    public:
        PerlinNoise();

        float perlin_noise(float sample_x, float sample_y, int length, int width);

    private:
        glm::vec2 get_constant_vector(int value);
        float lerp(float t, float a, float b);
        float fade(float t);

        int m_permutations[2 * PERMUTATION_SIZE];
};