#include "Game.h"


/* Game
 */
Game::Game():
p_shader_manager(ShaderManager::get_instance()),
p_texture_manager(TextureManager::get_instance())
{
    m_camera.set_position(glm::vec3(0, CHUNK_DEPTH + 5, 5));

    HeightMapGenerator::init();
}

/* process_mouse_input
 */
void Game::process_mouse_input(double x, double y) {
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
void Game::process_keyboard_input() {
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
void Game::setup() {
    // Load shaders
    p_shader_manager->load_shaders();

    // Load textures
    p_texture_manager->load_textures();

    // Create objects
    this->create_chunk(glm::vec2(0, 0));
    this->create_chunk(glm::vec2(1, 0));
    this->create_chunk(glm::vec2(-1, 0));
}

/* update
 */
void Game::update() {
    this->process_keyboard_input();

    if (m_mouse_updated) {
        m_camera.set_mouse_offset(m_mouse_offset_x, m_mouse_offset_y);
        m_mouse_updated = false;
    }
}

/* create_chunk
 */
void Game::create_chunk(glm::vec2 position) {
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
            Block *block = new Block(type, glm::vec3(0, 0, 0));

            m_instanced_objects[type] = new Object3DGroup(block);
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
bool Game::is_block_visible(glm::vec3 position) {
    return !(m_blocks.find(glm::vec3(position.x + 1, position.y, position.z)) != m_blocks.end() && // +x
             m_blocks.find(glm::vec3(position.x - 1, position.y, position.z)) != m_blocks.end() && // -x
             m_blocks.find(glm::vec3(position.x, position.y + 1, position.z)) != m_blocks.end() && // +y
             m_blocks.find(glm::vec3(position.x, position.y - 1, position.z)) != m_blocks.end() && // -y
             m_blocks.find(glm::vec3(position.x, position.y, position.z + 1)) != m_blocks.end() && // +z
             m_blocks.find(glm::vec3(position.x, position.y, position.z - 1)) != m_blocks.end());  // -z
}