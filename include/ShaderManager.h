#pragma once

// Includes
#include "Engine/Shader.h"

#include <unordered_map>

// Enums
/* eShader
 */
typedef enum {
    SHADER_BLOCK
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

    private:
        ShaderManager();

        static ShaderManager *p_instance;

        ShaderMap m_shader_map;
};