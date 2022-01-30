#include "PerlinNoise.h"


/* PerlinNoise
 */
PerlinNoise::PerlinNoise() {
    for (int i = 0; i < PERMUTATION_SIZE; i++) {
        m_permutations[i] = i;
    }

    for (int i = PERMUTATION_SIZE; i < 2 * PERMUTATION_SIZE; i++) {
        m_permutations[i] = i;
    }
}

/* perlin_noise
 */
float PerlinNoise::perlin_noise(float sample_x, float sample_y, int length, int width) {
    float xf = sample_x - std::floor(sample_x);
    float yf = sample_y - std::floor(sample_y);

    glm::vec2 top_right = glm::vec2(xf - 1, yf - 1);
    glm::vec2 top_left = glm::vec2(xf, yf - 1);
    glm::vec2 bottom_right = glm::vec2(xf - 1, yf);
    glm::vec2 bottom_left = glm::vec2(xf, yf);

    int X = (int)std::floor(sample_x) & (PERMUTATION_SIZE - 1);
    int Y = (int)std::floor(sample_y) & (PERMUTATION_SIZE - 1);

    int value_top_right = m_permutations[m_permutations[X + 1] + Y + 1];
    int value_top_left = m_permutations[m_permutations[X] + Y + 1];
    int value_bottom_right = m_permutations[m_permutations[X + 1] + Y];
    int value_bottom_left = m_permutations[m_permutations[X] + Y];

    float dot_top_right = glm::dot(top_right, get_constant_vector(value_top_right));
    float dot_top_left = glm::dot(top_left, get_constant_vector(value_top_left));
    float dot_bottom_right = glm::dot(bottom_right, get_constant_vector(value_bottom_right));
    float dot_bottom_left = glm::dot(bottom_left, get_constant_vector(value_bottom_left));

    float u = this->fade(xf);
    float v = this->fade(yf);

    return this->lerp(
        u,
        this->lerp(v, dot_bottom_left, dot_top_left),
        this->lerp(u, dot_bottom_right, dot_top_right)
    );
}

/* get_constant_vector
 */
glm::vec2 PerlinNoise::get_constant_vector(int value) {
    value = value % 4;
    
    switch(value) {
        case 0:
            return glm::vec2(1, 1);
        case 1:
            return glm::vec2(-1, 1);
        case 2:
            return glm::vec2(-1, -1);
        case 3:
            return glm::vec2(1, -1);
        default:
            break;
    }

    return glm::vec2(1, 1);
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