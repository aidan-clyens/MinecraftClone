#pragma once

// Includes
#include "Engine/Cube.h"

#include "ShaderManager.h"
#include "TextureManager.h"

#include <iostream>


/* Block
 */
class Block : public Cube {
    public:
        Block(eBlockType type, glm::vec3 position);

    private:
        eBlockType m_type;

        ShaderManager *p_shader_manager;
        TextureManager *p_texture_manager;
};