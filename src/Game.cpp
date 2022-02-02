#include "Game.h"


/* Game
 */
Game::Game():
m_world(this),
p_shader_manager(ShaderManager::get_instance()),
p_texture_manager(TextureManager::get_instance())
{
    m_camera.set_position(vec3(0, CHUNK_DEPTH + 5, 5));
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
    m_world.add_chunk(new Chunk(vec2(0, 0)));

    m_world.update_world_blocks();
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