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

    // Configure lighting
    m_material.ambient = WHITE;
    m_material.diffuse = WHITE;
    m_material.specular = WHITE;
    m_material.shininess = 1;

    m_light.ambient = glm::vec3(0.5, 0.5, 0.5);
    m_light.diffuse = glm::vec3(0.5, 0.5, 0.5);
    m_light.specular = glm::vec3(0.0, 0.0, 0.0);
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
Light ShaderManager::get_light() {
    return m_light;
}