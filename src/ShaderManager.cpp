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
    // Configure lighting
    m_material.ambient = WHITE;
    m_material.diffuse = WHITE;
    m_material.specular = WHITE;
    m_material.shininess = 1;

    m_light.set_direction(vec3(-0.2f, -1.0f, -0.3f));
    m_light.set_lighting(vec3(0.5, 0.5, 0.5), vec3(0.5, 0.5, 0.5), vec3(0.2, 0.2, 0.2));
}

/* get_shader
 */
Shader ShaderManager::get_shader(eShader type) {
    return m_shader_map[type];
}

/* get_material
 */
Material ShaderManager::get_material() {
    return m_material;
}

/* get_light
 */
DirectionalLight ShaderManager::get_light() {
    return m_light;
}
