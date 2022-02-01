#include "Game.h"


/* Game
 */
Game::Game():
p_shader_manager(ShaderManager::get_instance()),
p_texture_manager(TextureManager::get_instance())
{
    m_camera.set_position(vec3(0, CHUNK_DEPTH + 5, 5));

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
    this->create_chunk(vec2(0, 0));
    this->create_chunk(vec2(1, 0));
    this->create_chunk(vec2(-1, 0));

    this->update_world_blocks();
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
void Game::create_chunk(vec2 position) {
    Chunk chunk(position);
    BlockTypeMap *blocks = chunk.get_block_map();

    for (BlockTypeMapIterator it = blocks->begin(); it != blocks->end(); it++) {
        vec3 block_position = it->first;
        eBlockType type = it->second;

        m_world_blocks[block_position] = type;

        if (m_instanced_objects.find(type) == m_instanced_objects.end()) {
            Block *block = new Block(type, vec3(0, 0, 0));

            m_instanced_objects[type] = new Object3DGroup(block);
            this->add_object(m_instanced_objects[type]);
        }
    }
}

/* update_world_blocks
 */
void Game::update_world_blocks() {
    for (BlockTypeMapIterator it = m_world_blocks.begin(); it != m_world_blocks.end(); it++) {
        vec3 block_position = it->first;
        eBlockType type = it->second;

        // Determine if block is visible
        if (this->is_block_visible(block_position)) {
            Transform transform;
            transform.position = block_position;
            transform.rotation = vec3(0, 0, 0);
            transform.size = vec3(1, 1, 1);

            m_instanced_objects[type]->add_transform(transform);
        }
    }
}

/* is_block_visible
 */
bool Game::is_block_visible(vec3 position) {
    return !(m_world_blocks.find(vec3(position.x + 1, position.y, position.z)) != m_world_blocks.end() && // +x
             m_world_blocks.find(vec3(position.x - 1, position.y, position.z)) != m_world_blocks.end() && // -x
             m_world_blocks.find(vec3(position.x, position.y + 1, position.z)) != m_world_blocks.end() && // +y
             m_world_blocks.find(vec3(position.x, position.y - 1, position.z)) != m_world_blocks.end() && // -y
             m_world_blocks.find(vec3(position.x, position.y, position.z + 1)) != m_world_blocks.end() && // +z
             m_world_blocks.find(vec3(position.x, position.y, position.z - 1)) != m_world_blocks.end());  // -z
}