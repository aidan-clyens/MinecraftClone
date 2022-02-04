#include "Player.h"


/* Player
 */
Player::Player(vec3 starting_position):
Object3D(starting_position, vec3(0, 0, 0), vec3(1, 1, 1)),
p_input_manager(InputManager::get_instance()),
m_camera(starting_position)
{

}

/* update
 */
void Player::update(float delta_time) {
    const float speed = CAMERA_SPEED * delta_time;

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

    if (p_input_manager->is_mouse_updated()) {
        vec2 mouse_pos = p_input_manager->get_mouse_position();

        m_camera.set_mouse_offset(mouse_pos.x, mouse_pos.y);
        p_input_manager->set_mouse_handled(true);
    }
}

/* get_camera
 */
Camera *Player::get_camera() {
    return &m_camera;
}