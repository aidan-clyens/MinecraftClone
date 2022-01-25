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

#define CHUNK_WIDTH 16

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
            Shader block_shader("lib/3DEngine/shaders/vertex.glsl", "lib/3DEngine/shaders/texture_fragment.glsl");
            m_shader = block_shader;

            // Load textures
            Texture2D grass_texture("textures/grass.png", 0);
            m_grass_texture = grass_texture;

            // Configure lighting
            m_material.ambient = WHITE;
            m_material.diffuse = WHITE;
            m_material.specular = WHITE;
            m_material.shininess = 32;

            m_light.ambient = glm::vec3(0.5, 0.5, 0.5);
            m_light.diffuse = glm::vec3(0.5, 0.5, 0.5);
            m_light.specular = glm::vec3(0.0, 0.0, 0.0);

            // Create objects
            this->create_chunk(CHUNK_WIDTH, CHUNK_WIDTH);
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

        /* create_chunk
         */
        void create_chunk(int length, int width) {
            for (int x = 0; x < length; x++) {
                for (int z = 0; z < width; z++) {
                    Object3D *cube = new Object3D(glm::vec3(x, 0, z), glm::vec3(0, 0, 0), glm::vec3(1, 1, 1));
                    if (m_shader.is_valid()) {
                        cube->set_shader(m_shader);
                        cube->set_texture(m_grass_texture);
                        cube->set_material(m_material);
                        cube->set_light(m_light);
                    }
                    else {
                        std::cerr << "Error: Block shader invalid" << std::endl;
                    }

                    this->add_object(cube);
                }
            }
        }

    private:
        Shader m_shader;

        // Textures
        Texture2D m_grass_texture;

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