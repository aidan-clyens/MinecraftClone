// Includes
#include "Engine/Engine.h"
#include "Engine/Object3D.h"
#include "Engine/Shader.h"
#include "Engine/Texture2D.h"

#include <iostream>

// Defines
#define SCREEN_WIDTH 1600
#define SCREEN_HEIGHT 900

#define MOUSE_SENSITIVITY 0.1

#define WHITE glm::vec3(1, 1, 1)
#define ORANGE glm::vec3(1, 0.5, 0.31)

// Class definitions
/* Game
 */
class Game : public Engine {
    public:
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
            const float speed = 2.5 * m_delta_time;

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
            Shader block_shader("lib/3DEngine/shaders/vertex.glsl", "lib/3DEngine/shaders/color_fragment.glsl");

            // Configure lighting
            Material material;
            material.ambient = ORANGE;
            material.diffuse = ORANGE;
            material.specular = WHITE;
            material.shininess = 32;

            Light light;
            light.ambient = glm::vec3(0.5, 0.5, 0.5);
            light.diffuse = glm::vec3(0.5, 0.5, 0.5);
            light.specular = glm::vec3(0.2, 0.2, 0.2);

            // Create light source
            p_light = new Object3D(glm::vec3(2, 2, -5), glm::vec3(0, 0, 0), glm::vec3(1, 1, 1));
            if (block_shader.is_valid()) {
                p_light->set_shader(block_shader);
            }
            else {
                std::cerr << "Error: Block shader invalid" << std::endl;
            }

            this->add_object(p_light);
            this->add_light(p_light);

            // Create objects
            p_cube = new Object3D(glm::vec3(0, -0.5, -1.5), m_rotation, glm::vec3(1, 1, 1));
            if (block_shader.is_valid()) {
                p_cube->set_shader(block_shader);
                p_cube->set_material(material);
                p_cube->set_light(light);
            }
            else {
                std::cerr << "Error: Block shader invalid" << std::endl;
            }

            this->add_object(p_cube);
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

    private:
        Object3D *p_light;
        Object3D *p_cube;
        glm::vec3 m_rotation = glm::vec3(0, 0, 0);

        double m_rotation_speed = 25;

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