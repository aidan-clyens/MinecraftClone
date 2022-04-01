// Includes
#include "Engine/Engine.h"

#include "ShaderManager.h"
#include "TextureManager.h"
#include "Chunk.h"
#include "World.h"
#include "Player.h"

#include <iostream>

// Defines
#define SCREEN_WIDTH 1600
#define SCREEN_HEIGHT 900



// Class definitions
/* Game
 */
class Game : public Engine {
    public:
        Game();

        void process_keyboard_input();

        void setup();
        void update();

    private:
        // Objects
        World m_world;

        Player *p_player;

        // Managers
        ShaderManager *p_shader_manager;
        TextureManager *p_texture_manager;
};