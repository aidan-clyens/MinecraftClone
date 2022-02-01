// Includes
#include "Engine/Engine.h"
#include "Engine/Object3DGroup.h"

#include "Terrain/HeightMapGenerator.h"
#include "ShaderManager.h"
#include "TextureManager.h"
#include "Block.h"
#include "Chunk.h"

#include <iostream>
#include <unordered_map>
#include <cmath>

// Defines
#define SCREEN_WIDTH 1600
#define SCREEN_HEIGHT 900

#define MOUSE_SENSITIVITY 0.1

#define CAMERA_SPEED 5


// Typedefs
typedef std::unordered_map<eBlockType, Object3DGroup*> InstancedObjectMap;
typedef std::unordered_map<eBlockType, Object3DGroup*>::iterator InstancedObjectMapIterator;


// Class definitions
/* Game
 */
class Game : public Engine {
    public:
        Game();

        void process_mouse_input(double x, double y);
        void process_keyboard_input();

        void setup();
        void update();

        void create_chunk(vec2 position);

        void update_world_blocks();

        bool is_block_visible(vec3 position);

    private:
        // Objects
        InstancedObjectMap m_instanced_objects;
        BlockTypeMap m_world_blocks;

        // Managers
        ShaderManager *p_shader_manager;
        TextureManager *p_texture_manager;

        // Mouse
        bool m_first_mouse = false;
        double m_last_mouse_pos_x = SCREEN_WIDTH / 2;
        double m_last_mouse_pos_y = SCREEN_HEIGHT / 2;
        double m_mouse_offset_x = 0;
        double m_mouse_offset_y = 0;
        bool m_mouse_updated = false;
};