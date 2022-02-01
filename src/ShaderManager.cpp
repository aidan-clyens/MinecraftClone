#include "ShaderManager.h"


ShaderManager *ShaderManager::p_instance = 0;

/* ShaderManager
 */
ShaderManager::ShaderManager() {
    m_shader_map[SHADER_BLOCK] = Shader();
}

/* get_instance
 */
ShaderManager *ShaderManager::get_instance() {
    if (p_instance == 0) {
        p_instance = new ShaderManager();
    }

    return p_instance;
}

/* load_shaders
 */
void ShaderManager::load_shaders() {
    m_shader_map[SHADER_BLOCK].load("lib/3DEngine/shaders/vertex.glsl", "lib/3DEngine/shaders/cubemap_fragment.glsl");
}

/* get_shader
 */
Shader ShaderManager::get_shader(eShader type) {
    return m_shader_map[type];
}