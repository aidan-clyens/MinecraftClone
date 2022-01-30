// Includes
#include "Engine/Engine.h"
#include "Engine/Cube.h"
#include "Engine/Shader.h"
#include "Engine/Texture2D.h"
#include "Engine/TextureCubeMap.h"
#include "Engine/Object3DGroup.h"

#include "PerlinNoise.h"
#include "BlockAtlas.h"

#include <iostream>
#include <unordered_map>
#include <cmath>

// Defines
#define SCREEN_WIDTH 1600
#define SCREEN_HEIGHT 900

#define MOUSE_SENSITIVITY 0.1

#define CAMERA_SPEED 5

#define WHITE glm::vec3(1, 1, 1)

#define CHUNK_WIDTH 16
#define CHUNK_DEPTH 64

#define TEXTURE_WIDTH 16

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

// Enums
typedef enum {
    BLOCK_GRASS,
    BLOCK_DIRT
} eBlockType;

typedef enum {
    GRASS_SIDE,
    GRASS_BOTTOM,
    GRASS_TOP
} eBlockAtlas;

// Typedefs
// typedef std::unordered_map<glm::vec3, eBlockType, key_hash, key_equal> BlockMap;
// typedef std::unordered_map<glm::vec3, eBlockType, key_hash, key_equal>::iterator BlockMapIterator;

typedef std::unordered_map<eBlockType, std::vector<glm::vec3>> BlockMap;
typedef std::unordered_map<eBlockType, std::vector<glm::vec3>>::iterator BlockMapIterator;

// Class definitions
/* Game
 */
class Game : public Engine {
    public:
        /* Game
         */
        Game():
        m_block_atlas("textures/block_atlas.png", 8, 8, TEXTURE_WIDTH)
        {
            m_camera.set_position(glm::vec3(0, CHUNK_DEPTH + 5, 5));
        }

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
            const float speed = CAMERA_SPEED * m_delta_time;

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
            m_shader.load("lib/3DEngine/shaders/vertex.glsl", "lib/3DEngine/shaders/cubemap_fragment.glsl");

            // Load textures
            // Grass texture
            std::vector<unsigned char*> faces;
            faces.push_back(m_block_atlas.get_texture_data(GRASS_SIDE));
            faces.push_back(m_block_atlas.get_texture_data(GRASS_SIDE));
            faces.push_back(m_block_atlas.get_texture_data(GRASS_TOP));
            faces.push_back(m_block_atlas.get_texture_data(GRASS_BOTTOM));
            faces.push_back(m_block_atlas.get_texture_data(GRASS_SIDE));
            faces.push_back(m_block_atlas.get_texture_data(GRASS_SIDE));
            m_grass_texture_cube.load(faces, TEXTURE_WIDTH, TEXTURE_WIDTH, m_block_atlas.get_num_channels(), 0);

            // Dirt texture
            faces.clear();
            faces.push_back(m_block_atlas.get_texture_data(GRASS_BOTTOM));
            faces.push_back(m_block_atlas.get_texture_data(GRASS_BOTTOM));
            faces.push_back(m_block_atlas.get_texture_data(GRASS_BOTTOM));
            faces.push_back(m_block_atlas.get_texture_data(GRASS_BOTTOM));
            faces.push_back(m_block_atlas.get_texture_data(GRASS_BOTTOM));
            faces.push_back(m_block_atlas.get_texture_data(GRASS_BOTTOM));
            m_dirt_texture_cube.load(faces, TEXTURE_WIDTH, TEXTURE_WIDTH, m_block_atlas.get_num_channels(), 1);

            // Configure lighting
            m_material.ambient = WHITE;
            m_material.diffuse = WHITE;
            m_material.specular = WHITE;
            m_material.shininess = 32;

            m_light.ambient = glm::vec3(0.5, 0.5, 0.5);
            m_light.diffuse = glm::vec3(0.5, 0.5, 0.5);
            m_light.specular = glm::vec3(0.0, 0.0, 0.0);

            // Create objects
            for (int i = -1; i <= 1; i++) {
                for (int j = -1; j <= 1; j++) {
                    this->create_chunk(glm::vec2(i, j));
                }
            }
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
        void create_chunk(glm::vec2 position) {
            int seed = 123456;
            int scale = 1.5;
            int map_depth = 2;

            BlockMap blocks;

            // Determine block positions
            for (int x = 0; x < CHUNK_WIDTH; x++) {
                for (int z = 0; z < CHUNK_WIDTH; z++) {
                    int offset_x = (int)(position.x * (CHUNK_WIDTH - 1)) + seed;
                    int offset_z = (int)(position.y * (CHUNK_WIDTH - 1)) + seed;

                    float sample_x = (float)(x + offset_x) / (float)CHUNK_WIDTH * scale;
                    float sample_z = (float)(z + offset_z) / (float)CHUNK_WIDTH * scale;

                    int max_height = CHUNK_DEPTH + (int)std::floor(m_perlin.perlin_noise(sample_x, sample_z, CHUNK_WIDTH, CHUNK_WIDTH) * map_depth);

                    for (int y = 0; y < max_height; y++) {
                        eBlockType type;
                        if (y == max_height - 1) {
                            type = BLOCK_GRASS;
                        }
                        else {
                            type = BLOCK_DIRT;
                        }

                        blocks[type].push_back(glm::vec3(x + position.x * (CHUNK_WIDTH - 1), y, z + position.y * (CHUNK_WIDTH - 1)));
                    }
                }
            }

            // Create an Object3DGroup for each type of block
            for (BlockMapIterator it = blocks.begin(); it != blocks.end(); it++) {
                std::vector<Transform> transforms;
                Cube *cube = new Cube(glm::vec3(0, 0, 0), glm::vec3(0, 0, 0), glm::vec3(1, 1, 1));

                if (m_shader.is_valid()) {
                    cube->set_shader(m_shader);

                    switch (it->first) {
                        case BLOCK_GRASS:
                            cube->set_texture(m_grass_texture_cube);
                            break;

                        case BLOCK_DIRT:
                        default:
                            cube->set_texture(m_dirt_texture_cube);
                            break;
                    }

                    cube->set_material(m_material);
                    cube->set_light(m_light);
                }
                else {
                    std::cerr << "Error: Block shader invalid" << std::endl;
                }

                for (int i = 0; i < it->second.size(); i++) {
                    Transform transform;
                    transform.position = it->second[i];
                    transform.rotation = glm::vec3(0, 0, 0);
                    transform.size = glm::vec3(1, 1, 1);

                    transforms.push_back(transform);
                }

                Object3DGroup *group = new Object3DGroup(cube, transforms);
                this->add_object(group);
            }
        }

    private:
        // Noise
        PerlinNoise m_perlin;

        // Shaders
        Shader m_shader;

        // Textures
        BlockAtlas m_block_atlas;
        TextureCubeMap m_grass_texture_cube;
        TextureCubeMap m_dirt_texture_cube;

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