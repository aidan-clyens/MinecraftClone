#include "TextureManager.h"


TextureManager *TextureManager::p_instance = 0;

/* TextureManager
 */
TextureManager::TextureManager():
m_block_atlas("textures/block_atlas.png", 8, 8, TEXTURE_WIDTH)
{
    m_texture_map[BLOCK_GRASS] = TextureCubeMap();
    m_texture_map[BLOCK_DIRT] = TextureCubeMap();
    m_texture_map[BLOCK_STONE] = TextureCubeMap();
}

/* get_instance
 */
TextureManager *TextureManager::get_instance() {
    if (p_instance == 0) {
        p_instance = new TextureManager();
    }

    return p_instance;
}

/* load_textures
 */
void TextureManager::load_textures() {
    // Grass texture
    std::vector<unsigned char*> faces;
    faces.push_back(m_block_atlas.get_texture_data(GRASS_SIDE));
    faces.push_back(m_block_atlas.get_texture_data(GRASS_SIDE));
    faces.push_back(m_block_atlas.get_texture_data(GRASS_TOP));
    faces.push_back(m_block_atlas.get_texture_data(GRASS_BOTTOM));
    faces.push_back(m_block_atlas.get_texture_data(GRASS_SIDE));
    faces.push_back(m_block_atlas.get_texture_data(GRASS_SIDE));
    m_texture_map[BLOCK_GRASS].load(faces, TEXTURE_WIDTH, TEXTURE_WIDTH, m_block_atlas.get_num_channels());

    // Dirt texture
    faces.clear();
    faces.push_back(m_block_atlas.get_texture_data(GRASS_BOTTOM));
    faces.push_back(m_block_atlas.get_texture_data(GRASS_BOTTOM));
    faces.push_back(m_block_atlas.get_texture_data(GRASS_BOTTOM));
    faces.push_back(m_block_atlas.get_texture_data(GRASS_BOTTOM));
    faces.push_back(m_block_atlas.get_texture_data(GRASS_BOTTOM));
    faces.push_back(m_block_atlas.get_texture_data(GRASS_BOTTOM));
    m_texture_map[BLOCK_DIRT].load(faces, TEXTURE_WIDTH, TEXTURE_WIDTH, m_block_atlas.get_num_channels());

    // Stone texture
    faces.clear();
    faces.push_back(m_block_atlas.get_texture_data(STONE));
    faces.push_back(m_block_atlas.get_texture_data(STONE));
    faces.push_back(m_block_atlas.get_texture_data(STONE));
    faces.push_back(m_block_atlas.get_texture_data(STONE));
    faces.push_back(m_block_atlas.get_texture_data(STONE));
    faces.push_back(m_block_atlas.get_texture_data(STONE));
    m_texture_map[BLOCK_STONE].load(faces, TEXTURE_WIDTH, TEXTURE_WIDTH, m_block_atlas.get_num_channels());
}

/* get_texture
 */
TextureCubeMap TextureManager::get_texture(eBlockType type) {
    return (TextureCubeMap)m_texture_map[type];
}