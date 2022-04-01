#include "Player.h"


/* Player
 */
Player::Player(vec3 starting_position, bool fly_mode):
Object3D(starting_position, vec3(0, 0, 0), vec3(1, 2, 1)),
p_input_manager(InputManager::get_instance()),
m_camera(starting_position),
m_fly_mode(fly_mode)
{
    this->add_component(COMP_RIGIDBODY, new DynamicCharacterController(this, 1));
}

/* update
 */
void Player::update(float delta_time) {
    DynamicCharacterController *controller = (DynamicCharacterController*)this->get_component(COMP_RIGIDBODY);

    if (m_fly_mode) {
        const float speed = 4.0 * delta_time;

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
    }
    else {
        const float speed = 80.0 * delta_time;

        vec2 front = vec2(m_camera.front().x, m_camera.front().z);
        vec2 right = vec2(m_camera.right().x, m_camera.right().z);
        vec2 player_direction = vec2(0, 0);

        if (p_input_manager->get_key(KEY_W) == KEY_PRESS) {
            player_direction += front * speed;
        }
        else if (p_input_manager->get_key(KEY_S) == KEY_PRESS) {
            player_direction += front * -speed;
        }

        if (p_input_manager->get_key(KEY_A) == KEY_PRESS) {
            player_direction += right * -speed;
        }
        else if (p_input_manager->get_key(KEY_D) == KEY_PRESS) {
            player_direction += right * speed;
        }

        if (p_input_manager->get_key(KEY_SPACE) == KEY_PRESS) {
            controller->jump();
        }

        controller->move(player_direction);
        m_camera.set_position(m_transform.position);
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