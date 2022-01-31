// Includes
#include "Engine/Engine.h"
#include "Engine/Cube.h"
#include "Engine/Shader.h"
#include "Engine/Texture2D.h"
#include "Engine/TextureCubeMap.h"
#include "Engine/Object3DGroup.h"

#include "HeightMapGenerator.h"
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

// Enums
typedef enum {
    SHADER_BLOCK
} eShader;

typedef enum {
    BLOCK_GRASS,
    BLOCK_DIRT,
    BLOCK_STONE
} eBlockType;

typedef enum {
    GRASS_SIDE,
    GRASS_BOTTOM,
    GRASS_TOP,
    STONE
} eBlockAtlas;

// Structs
/* key_hash
 */
struct vec3_key_hash : public std::unary_function<glm::vec3, std::size_t> {
    std::size_t operator() (const glm::vec3 &c) const {
        int x = (int)c.x;
        int y = (int)c.y;
        int z = (int)c.z;
        return x ^ y ^ z;
    }
};

/* key_equal
 */
struct vec3_key_equal : public std::binary_function<glm::vec3, glm::vec3, bool> {
    bool operator() (const glm::vec3 &c0, const glm::vec3 &c1) const {
        return c0 == c1;
    }
};

// Typedefs
typedef std::unordered_map<eBlockType, TextureCubeMap> TextureMap;
typedef std::unordered_map<eBlockType, TextureCubeMap>::iterator TextureMapIterator;

typedef std::unordered_map<eShader, Shader> ShaderMap;
typedef std::unordered_map<eShader, Shader>::iterator ShaderMapIterator;

typedef std::unordered_map<eBlockType, std::vector<glm::vec3>> BlockMap;
typedef std::unordered_map<eBlockType, std::vector<glm::vec3>>::iterator BlockMapIterator;

typedef std::unordered_map<eBlockType, Object3DGroup*> InstancedObjectMap;
typedef std::unordered_map<eBlockType, Object3DGroup*>::iterator InstancedObjectMapIterator;

typedef std::unordered_map<glm::vec3, eBlockType, vec3_key_hash, vec3_key_equal> BlockPositionMap;


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

            m_shader_map[SHADER_BLOCK] = Shader();

            m_texture_map[BLOCK_GRASS] = TextureCubeMap();
            m_texture_map[BLOCK_DIRT] = TextureCubeMap();
            m_texture_map[BLOCK_STONE] = TextureCubeMap();

            HeightMapGenerator::init();
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
            m_shader_map[SHADER_BLOCK].load("lib/3DEngine/shaders/vertex.glsl", "lib/3DEngine/shaders/cubemap_fragment.glsl");

            // Load textures
            this->load_textures();

            // Configure lighting
            m_material.ambient = WHITE;
            m_material.diffuse = WHITE;
            m_material.specular = WHITE;
            m_material.shininess = 32;

            m_light.ambient = glm::vec3(0.5, 0.5, 0.5);
            m_light.diffuse = glm::vec3(0.5, 0.5, 0.5);
            m_light.specular = glm::vec3(0.0, 0.0, 0.0);

            // Create objects
            this->create_chunk(glm::vec2(0, 0));
            this->create_chunk(glm::vec2(1, 0));
            this->create_chunk(glm::vec2(-1, 0));
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

        /* load_textures
         */
        void load_textures() {
            // Grass texture
            std::vector<unsigned char*> faces;
            faces.push_back(m_block_atlas.get_texture_data(GRASS_SIDE));
            faces.push_back(m_block_atlas.get_texture_data(GRASS_SIDE));
            faces.push_back(m_block_atlas.get_texture_data(GRASS_TOP));
            faces.push_back(m_block_atlas.get_texture_data(GRASS_BOTTOM));
            faces.push_back(m_block_atlas.get_texture_data(GRASS_SIDE));
            faces.push_back(m_block_atlas.get_texture_data(GRASS_SIDE));
            m_texture_map[BLOCK_GRASS].load(faces, TEXTURE_WIDTH, TEXTURE_WIDTH, m_block_atlas.get_num_channels(), 0);

            // Dirt texture
            faces.clear();
            faces.push_back(m_block_atlas.get_texture_data(GRASS_BOTTOM));
            faces.push_back(m_block_atlas.get_texture_data(GRASS_BOTTOM));
            faces.push_back(m_block_atlas.get_texture_data(GRASS_BOTTOM));
            faces.push_back(m_block_atlas.get_texture_data(GRASS_BOTTOM));
            faces.push_back(m_block_atlas.get_texture_data(GRASS_BOTTOM));
            faces.push_back(m_block_atlas.get_texture_data(GRASS_BOTTOM));
            m_texture_map[BLOCK_DIRT].load(faces, TEXTURE_WIDTH, TEXTURE_WIDTH, m_block_atlas.get_num_channels(), 1);

            // Stone texture
            faces.clear();
            faces.push_back(m_block_atlas.get_texture_data(STONE));
            faces.push_back(m_block_atlas.get_texture_data(STONE));
            faces.push_back(m_block_atlas.get_texture_data(STONE));
            faces.push_back(m_block_atlas.get_texture_data(STONE));
            faces.push_back(m_block_atlas.get_texture_data(STONE));
            faces.push_back(m_block_atlas.get_texture_data(STONE));
            m_texture_map[BLOCK_STONE].load(faces, TEXTURE_WIDTH, TEXTURE_WIDTH, m_block_atlas.get_num_channels(), 2);
        }

        /* create_chunk
         */
        void create_chunk(glm::vec2 position) {
            BlockMap blocks;

            // Generate height map
            HeightMapSettings settings;
            settings.seed = 123456;
            settings.width = CHUNK_WIDTH;
            settings.height = CHUNK_WIDTH;
            settings.scale = 1;
            settings.depth = 5;
            settings.octaves = 3;
            settings.persistence = 0.2;
            settings.lacunarity = 2;

            HeightMap height_map;
            HeightMapGenerator::generate_height_map(height_map, position.x, position.y, settings);

            for (int x = 0; x < CHUNK_WIDTH; x++) {
                for (int z = 0; z < CHUNK_WIDTH; z++) {
                    int max_height = CHUNK_DEPTH + (int)std::floor(height_map[glm::vec2(x, z)]);

                    for (int y = 0; y < max_height; y++) {
                        eBlockType type;
                        if (y == max_height - 1) {
                            type = BLOCK_GRASS;
                        }
                        else if (y < max_height - 1 && y > max_height - 5) {
                            type = BLOCK_DIRT;
                        }
                        else {
                            type = BLOCK_STONE;
                        }

                        blocks[type].push_back(glm::vec3(x + position.x * (CHUNK_WIDTH - 1), y, z + position.y * (CHUNK_WIDTH - 1)));
                        m_blocks[glm::vec3(x + position.x * (CHUNK_WIDTH - 1), y, z + position.y * (CHUNK_WIDTH - 1))] = type;
                    }
                }
            }

            // Create an Object3DGroup for each type of block
            for (BlockMapIterator it = blocks.begin(); it != blocks.end(); it++) {
                eBlockType type = it->first;

                if (m_instanced_objects.find(type) == m_instanced_objects.end()) {
                    Cube *cube = new Cube(glm::vec3(0, 0, 0), glm::vec3(0, 0, 0), glm::vec3(1, 1, 1));

                    if (m_shader_map[SHADER_BLOCK].is_valid()) {
                        cube->set_shader(m_shader_map[SHADER_BLOCK]);
                        cube->set_texture(m_texture_map[type]);
                        cube->set_material(m_material);
                        cube->set_light(m_light);
                    }
                    else {
                        std::cerr << "Error: Block shader invalid" << std::endl;
                    }

                    m_instanced_objects[type] = new Object3DGroup(cube);
                    this->add_object(m_instanced_objects[type]);
                }

                for (int i = 0; i < it->second.size(); i++) {
                    // Determine if block is visible
                    if (this->is_block_visible(it->second[i])) {
                        Transform transform;
                        transform.position = it->second[i];
                        transform.rotation = glm::vec3(0, 0, 0);
                        transform.size = glm::vec3(1, 1, 1);

                        m_instanced_objects[type]->add_transform(transform);
                    }
                }
            }
        }

        /* is_block_visible
         */
        bool is_block_visible(glm::vec3 position) {
            return !(m_blocks.find(glm::vec3(position.x + 1, position.y, position.z)) != m_blocks.end() && // +x
                     m_blocks.find(glm::vec3(position.x - 1, position.y, position.z)) != m_blocks.end() && // -x
                     m_blocks.find(glm::vec3(position.x, position.y + 1, position.z)) != m_blocks.end() && // +y
                     m_blocks.find(glm::vec3(position.x, position.y - 1, position.z)) != m_blocks.end() && // -y
                     m_blocks.find(glm::vec3(position.x, position.y, position.z + 1)) != m_blocks.end() && // +z
                     m_blocks.find(glm::vec3(position.x, position.y, position.z - 1)) != m_blocks.end());  // -z
        }

    private:
        // Objects
        InstancedObjectMap m_instanced_objects;

        BlockPositionMap m_blocks;

        // Shaders
        ShaderMap m_shader_map;

        // Textures
        BlockAtlas m_block_atlas;
        TextureMap m_texture_map;

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