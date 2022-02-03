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

/* process_keyboard_input
 */
void Game::process_keyboard_input() {
    const float speed = CAMERA_SPEED * m_delta_time;

    if (p_input_manager->get_key(KEY_W) == KEY_PRESS) {
        m_camera.translate_x(speed);
    }
    if (p_input_manager->get_key(KEY_S) == KEY_PRESS) {
        m_camera.translate_x(-speed);
    }
    if (p_input_manager->get_key(KEY_A) == KEY_PRESS) {
        m_camera.translate_z(-speed);
    }
    if (p_input_manager->get_key(KEY_D) == KEY_PRESS) {
        m_camera.translate_z(speed);
    }
    if (p_input_manager->get_key(KEY_SPACE) == KEY_PRESS) {
        m_camera.translate_y(speed);
    }
    if (p_input_manager->get_key(KEY_LEFT_SHIFT) == KEY_PRESS) {
        m_camera.translate_y(-speed);
    }

    if (p_input_manager->get_key(KEY_ESCAPE) == KEY_PRESS) {
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

    if (p_input_manager->is_mouse_updated()) {
        vec2 mouse_pos = p_input_manager->get_mouse_position();

        m_camera.set_mouse_offset(mouse_pos.x, mouse_pos.y);
        p_input_manager->set_mouse_handled(true);
    }
}