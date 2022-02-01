#include "Block.h"


/* Block
 */
Block::Block(eBlockType type, glm::vec3 position):
Cube(position, glm::vec3(0, 0, 0), glm::vec3(1, 1, 1)),
m_type(type),
p_shader_manager(ShaderManager::get_instance()),
p_texture_manager(TextureManager::get_instance())
{
    if (p_shader_manager->get_shader(SHADER_BLOCK).is_valid()) {
        this->set_shader(p_shader_manager->get_shader(SHADER_BLOCK));
        this->set_texture(p_texture_manager->get_texture(m_type));
        this->set_material(p_shader_manager->get_material());
        this->set_light(p_shader_manager->get_light());
    }
    else {
        std::cerr << "Error: Block shader invalid" << std::endl;
    }
}