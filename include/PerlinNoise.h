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

        float perlin_noise(int x, int y, int length, int width, int scale);

    private:
        glm::vec2 get_constant_vector(int value);
        float lerp(float t, float a, float b);
        float fade(float t);

        int m_permutations[PERMUTATION_SIZE];
};