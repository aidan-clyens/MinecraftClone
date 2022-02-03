#include "Game.h"


/* Game
 */
Game::Game():
m_world(this),
m_player(vec3(0, CHUNK_DEPTH + 5, 5)),
p_shader_manager(ShaderManager::get_instance()),
p_texture_manager(TextureManager::get_instance())
{
    this->set_camera(m_player.get_camera());
}

/* process_keyboard_input
 */
void Game::process_keyboard_input() {
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

    m_player.update(m_delta_time);
}