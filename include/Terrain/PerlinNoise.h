#pragma once

// Includes
#include "Engine/utils/types.h"

// Defines
#define PERMUTATION_SIZE 256


/* PerlinNoise
 */
class PerlinNoise {
    public:
        static void init();
        static float perlin_noise(float sample_x, float sample_y, int length, int width);

    private:
        PerlinNoise();

        static vec2 get_constant_vector(int value);
        static float lerp(float t, float a, float b);
        static float fade(float t);
};