#pragma once

// Includes
#include "Engine/Shader.h"
#include "Engine/Object3D.h"

#include <unordered_map>

// Defines
#define WHITE vec3(1, 1, 1)

// Enums
/* eShader
 */
typedef enum {

} eShader;

// Typedefs
typedef std::unordered_map<eShader, Shader> ShaderMap;
typedef std::unordered_map<eShader, Shader>::iterator ShaderMapIterator;

/* ShaderManager
 */
class ShaderManager {
    public:
        static ShaderManager *get_instance();

        void load_shaders();

        Shader get_shader(eShader type);
        Material get_material();
        Light get_light();

        vec3 get_light_vector() const;

    private:
        ShaderManager();

        static ShaderManager *p_instance;

        ShaderMap m_shader_map;
        Material m_material;
        Light m_light;

        vec3 m_light_vector;
};