#include "ShaderManager.h"


ShaderManager *ShaderManager::p_instance = 0;

/* ShaderManager
 */
ShaderManager::ShaderManager() {

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

}

/* get_shader
 */
Shader ShaderManager::get_shader(eShader type) {
    return m_shader_map[type];
}
