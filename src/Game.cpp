#include "Game.h"


/* Game
 */
Game::Game():
Engine("lib/3DEngine/"),
m_world(this),
p_player(new Player(vec3(5, CHUNK_DEPTH + 5, 5), true)),
p_shader_manager(ShaderManager::get_instance()),
p_texture_manager(TextureManager::get_instance())
{

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
    this->set_mouse_visible(false);
    this->set_camera(p_player->get_camera());
    this->set_shadows_enabled(true);

    this->add_object(p_player);

    DirectionalLight light;
    vec3 light_position = vec3(CHUNK_WIDTH / 2.0, CHUNK_DEPTH + 8.0f, CHUNK_WIDTH / 4.0);
    light.set_position(light_position);
    light.set_origin(vec3(0.0, CHUNK_DEPTH, 0.0));
    light.set_lighting(vec3(0.5, 0.5, 0.5), vec3(0.5, 0.5, 0.5), vec3(0.2, 0.2, 0.2));

    // Load shaders
    p_shader_manager->load_shaders();
    this->set_directional_light(light);

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

    p_player->update(m_delta_time);
}