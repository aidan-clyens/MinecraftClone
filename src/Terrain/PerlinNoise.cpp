#include "Terrain/PerlinNoise.h"


static int permutations[2 * PERMUTATION_SIZE];


/* PerlinNoise
 */
void PerlinNoise::init() {
    for (int i = 0; i < PERMUTATION_SIZE; i++) {
        permutations[i] = i;
    }

    for (int i = PERMUTATION_SIZE; i < 2 * PERMUTATION_SIZE; i++) {
        permutations[i] = i;
    }
}

/* perlin_noise
 */
float PerlinNoise::perlin_noise(float sample_x, float sample_y, int length, int width) {
    float xf = sample_x - std::floor(sample_x);
    float yf = sample_y - std::floor(sample_y);

    vec2 top_right = vec2(xf - 1, yf - 1);
    vec2 top_left = vec2(xf, yf - 1);
    vec2 bottom_right = vec2(xf - 1, yf);
    vec2 bottom_left = vec2(xf, yf);

    int X = (int)std::floor(sample_x) & (PERMUTATION_SIZE - 1);
    int Y = (int)std::floor(sample_y) & (PERMUTATION_SIZE - 1);

    int value_top_right = permutations[permutations[X + 1] + Y + 1];
    int value_top_left = permutations[permutations[X] + Y + 1];
    int value_bottom_right = permutations[permutations[X + 1] + Y];
    int value_bottom_left = permutations[permutations[X] + Y];

    float dot_top_right = glm::dot(top_right, get_constant_vector(value_top_right));
    float dot_top_left = glm::dot(top_left, get_constant_vector(value_top_left));
    float dot_bottom_right = glm::dot(bottom_right, get_constant_vector(value_bottom_right));
    float dot_bottom_left = glm::dot(bottom_left, get_constant_vector(value_bottom_left));

    float u = PerlinNoise::fade(xf);
    float v = PerlinNoise::fade(yf);

    return PerlinNoise::lerp(
        u,
        PerlinNoise::lerp(v, dot_bottom_left, dot_top_left),
        PerlinNoise::lerp(u, dot_bottom_right, dot_top_right)
    );
}

/* get_constant_vector
 */
vec2 PerlinNoise::get_constant_vector(int value) {
    value = value % 4;
    
    switch(value) {
        case 0:
            return vec2(1, 1);
        case 1:
            return vec2(-1, 1);
        case 2:
            return vec2(-1, -1);
        case 3:
            return vec2(1, -1);
        default:
            break;
    }

    return vec2(1, 1);
}

/* lerp
 */
float PerlinNoise::lerp(float t, float a, float b) {
    return a + t * (b - a);
}

/* fade
 */
float PerlinNoise::fade(float t) {
    return ((6 * t - 15) * t + 10) * t * t * t;
}