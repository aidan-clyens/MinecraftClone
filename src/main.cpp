// Includes
#include "Engine/Engine.h"
#include "Engine/Cube.h"
#include "Engine/Shader.h"
#include "Engine/Texture2D.h"

#include "PerlinNoise.h"

#include <iostream>
#include <unordered_map>
#include <cmath>

// Defines
#define SCREEN_WIDTH 1600
#define SCREEN_HEIGHT 900

#define MOUSE_SENSITIVITY 0.1

#define WHITE glm::vec3(1, 1, 1)

#define CHUNK_WIDTH 16

// Structs
/* key_hash
 */
struct key_hash : public std::unary_function<glm::vec3, std::size_t> {
    std::size_t operator() (const glm::vec3 &c) const {
        int x = (int)c.x;
        int y = (int)c.y;
        int z = (int)c.z;
        return x ^ y ^ z;
    }
};

/* key_equal
 */
struct key_equal : public std::binary_function<glm::vec3, glm::vec3, bool> {
    bool operator() (const glm::vec3 &c0, const glm::vec3 &c1) const {
        return c0 == c1;
    }
};

// Typedefs
typedef std::unordered_map<glm::vec3, Cube*, key_hash, key_equal> BlockMap;
typedef std::unordered_map<glm::vec3, Cube*, key_hash, key_equal>::iterator BlockMapIterator;


// Class definitions
/* Game
 */
class Game : public Engine {
    public:
        /* process_mouse_input
         */
        void process_mouse_input(double x, double y) {
            if (m_first_mouse) {
                m_last_mouse_pos_x = x;
                m_last_mouse_pos_y = y;
                m_first_mouse = false;
            }

            m_mouse_offset_x = x - m_last_mouse_pos_x;
            m_mouse_offset_y = m_last_mouse_pos_y - y;

            m_last_mouse_pos_x = x;
            m_last_mouse_pos_y = y;

            m_mouse_offset_x *= MOUSE_SENSITIVITY;
            m_mouse_offset_y *= MOUSE_SENSITIVITY;

            m_mouse_updated = true;
        }

        /* process_mouse_input
         */
        void process_keyboard_input() {
            const float speed = 2.5 * m_delta_time;

            if (get_key(KEY_W) == KEY_PRESS) {
                m_camera.translate_x(speed);
            }
            if (get_key(KEY_S) == KEY_PRESS) {
                m_camera.translate_x(-speed);
            }
            if (get_key(KEY_A) == KEY_PRESS) {
                m_camera.translate_z(-speed);
            }
            if (get_key(KEY_D) == KEY_PRESS) {
                m_camera.translate_z(speed);
            }
            if (get_key(KEY_SPACE) == KEY_PRESS) {
                m_camera.translate_y(speed);
            }
            if (get_key(KEY_LEFT_SHIFT) == KEY_PRESS) {
                m_camera.translate_y(-speed);
            }

            if (get_key(KEY_ESCAPE) == KEY_PRESS) {
                m_running = false;
            }
        }

        /* setup
         */
        void setup() {
            // Load shaders
            m_shader.load("lib/3DEngine/shaders/vertex.glsl", "lib/3DEngine/shaders/texture_fragment.glsl");

            // Load textures
            m_grass_texture.load("textures/grass.png", 0);

            // Configure lighting
            m_material.ambient = WHITE;
            m_material.diffuse = WHITE;
            m_material.specular = WHITE;
            m_material.shininess = 32;

            m_light.ambient = glm::vec3(0.5, 0.5, 0.5);
            m_light.diffuse = glm::vec3(0.5, 0.5, 0.5);
            m_light.specular = glm::vec3(0.0, 0.0, 0.0);

            // Create objects
            this->create_chunk(CHUNK_WIDTH, CHUNK_WIDTH);
        }

        /* update
         */
        void update() {
            this->process_keyboard_input();

            if (m_mouse_updated) {
                m_camera.set_mouse_offset(m_mouse_offset_x, m_mouse_offset_y);
                m_mouse_updated = false;
            }
        }

        /* create_chunk
         */
        void create_chunk(int length, int width) {
            int scale = 2;
            int map_depth = 1;
            glm::vec3 position;

            for (int x = 0; x < length; x++) {
                for (int z = 0; z < width; z++) {
                    int max_height = (int)std::floor(m_perlin.perlin_noise(x, z, length, width, scale) * map_depth);

                    for (int y = -10; y < max_height; y++) {
                        position = glm::vec3(x, y, z);
                        Cube *cube = new Cube(position, glm::vec3(0, 0, 0), glm::vec3(1, 1, 1));
                        if (m_shader.is_valid()) {
                            cube->set_shader(m_shader);
                            cube->set_texture(m_grass_texture);
                            cube->set_material(m_material);
                            cube->set_light(m_light);

                            m_blocks[position] = cube;
                        }
                        else {
                            std::cerr << "Error: Block shader invalid" << std::endl;
                        }

                        this->add_object(cube);
                    }
                }
            }

            // Update block faces
            for (BlockMapIterator it = m_blocks.begin(); it != m_blocks.end(); it++) {
                position = it->first;
                update_block_faces(position);
            }
        }

        /* update_block_faces
         */
        void update_block_faces(glm::vec3 position) {
            BlockMapIterator it;

            Cube *block;
            it = m_blocks.find(position);
            if (it != m_blocks.end()) {
                block = it->second;
            }
            else {
                return;
            }

            // Back face
            if (m_blocks.find(glm::vec3(position.x, position.y, position.z - 1)) != m_blocks.end()) {
                block->set_face_enabled(CUBE_BACK, false);
            }
            else {
                block->set_face_enabled(CUBE_BACK, true);
            }
            // Front face
            if (m_blocks.find(glm::vec3(position.x, position.y, position.z + 1)) != m_blocks.end()) {
                block->set_face_enabled(CUBE_FRONT, false);
            }
            else {
                block->set_face_enabled(CUBE_FRONT, true);
            }
            // Left face
            if (m_blocks.find(glm::vec3(position.x - 1, position.y, position.z)) != m_blocks.end()) {
                block->set_face_enabled(CUBE_LEFT, false);
            }
            else {
                block->set_face_enabled(CUBE_LEFT, true);
            }
            // Right face
            if (m_blocks.find(glm::vec3(position.x + 1, position.y, position.z)) != m_blocks.end()) {
                block->set_face_enabled(CUBE_RIGHT, false);
            }
            else {
                block->set_face_enabled(CUBE_RIGHT, true);
            }
            // Bottom face
            if (m_blocks.find(glm::vec3(position.x, position.y - 1, position.z)) != m_blocks.end()) {
                block->set_face_enabled(CUBE_BOTTOM, false);
            }
            else {
                block->set_face_enabled(CUBE_BOTTOM, true);
            }
            // Top face
            if (m_blocks.find(glm::vec3(position.x, position.y + 1, position.z)) != m_blocks.end()) {
                block->set_face_enabled(CUBE_TOP, false);
            }
            else {
                block->set_face_enabled(CUBE_TOP, true);
            }
        }

    private:
        // Objects
        BlockMap m_blocks;

        // Noise
        PerlinNoise m_perlin;

        // Shaders
        Shader m_shader;

        // Textures
        Texture2D m_grass_texture;

        // Materials
        Material m_material;

        // Light
        Light m_light;

        // Mouse
        bool m_first_mouse = false;
        double m_last_mouse_pos_x = SCREEN_WIDTH / 2;
        double m_last_mouse_pos_y = SCREEN_HEIGHT / 2;
        double m_mouse_offset_x = 0;
        double m_mouse_offset_y = 0;
        bool m_mouse_updated = false;
};

/* main
 */
int main(int argc, char **argv) {
    Game game;

    if (!game.init()) {
        std::cerr << "Game Engine failed to initialize" << std::endl;
        return -1;
    }

    game.start();
    game.cleanup();

    return 0;
}