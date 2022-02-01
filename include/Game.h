// Includes
#include "Engine/Engine.h"
#include "Engine/Cube.h"
#include "Engine/Shader.h"
#include "Engine/Texture2D.h"
#include "Engine/TextureCubeMap.h"
#include "Engine/Object3DGroup.h"

#include "HeightMapGenerator.h"
#include "BlockAtlas.h"
#include "ShaderManager.h"
#include "TextureManager.h"

#include <iostream>
#include <unordered_map>
#include <cmath>

// Defines
#define SCREEN_WIDTH 1600
#define SCREEN_HEIGHT 900

#define MOUSE_SENSITIVITY 0.1

#define CAMERA_SPEED 5

#define WHITE glm::vec3(1, 1, 1)

#define CHUNK_WIDTH 16
#define CHUNK_DEPTH 64


// Structs
/* key_hash
 */
struct vec3_key_hash : public std::unary_function<glm::vec3, std::size_t> {
    std::size_t operator() (const glm::vec3 &c) const {
        int x = (int)c.x;
        int y = (int)c.y;
        int z = (int)c.z;
        return x ^ y ^ z;
    }
};

/* key_equal
 */
struct vec3_key_equal : public std::binary_function<glm::vec3, glm::vec3, bool> {
    bool operator() (const glm::vec3 &c0, const glm::vec3 &c1) const {
        return c0 == c1;
    }
};

// Typedefs
typedef std::unordered_map<eBlockType, std::vector<glm::vec3>> BlockMap;
typedef std::unordered_map<eBlockType, std::vector<glm::vec3>>::iterator BlockMapIterator;

typedef std::unordered_map<eBlockType, Object3DGroup*> InstancedObjectMap;
typedef std::unordered_map<eBlockType, Object3DGroup*>::iterator InstancedObjectMapIterator;

typedef std::unordered_map<glm::vec3, eBlockType, vec3_key_hash, vec3_key_equal> BlockPositionMap;


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

        void create_chunk(glm::vec2 position);

        bool is_block_visible(glm::vec3 position);

    private:
        // Objects
        InstancedObjectMap m_instanced_objects;

        BlockPositionMap m_blocks;

        // Managers
        ShaderManager *p_shader_manager;
        TextureManager *p_texture_manager;

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