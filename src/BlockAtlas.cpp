#include "BlockAtlas.h"


/* BlockAtlas
 */
BlockAtlas::BlockAtlas(const std::string &path, int cols, int rows, int texture_width):
m_cols(cols),
m_rows(rows),
m_texture_width(texture_width)
{
    p_data = load_image_data(path, &m_atlas_width, &m_atlas_height, &m_num_channels);
    int data_size = m_atlas_width * m_atlas_height * m_num_channels;

    const int num_textures = m_cols * m_rows;
    const int texture_data_size = m_texture_width * m_texture_width * m_num_channels;

    // Split block atlas into individual textures
    for (int y = 0; y < (m_atlas_height * m_num_channels); y++) {
        int row = (int)(y / (m_texture_width * m_num_channels));
        for (int x = 0; x < (m_atlas_width * m_num_channels); x++) {
            int pixel_index = x + y * (m_atlas_width * m_num_channels);
            int col = (int)(x / (m_texture_width * m_num_channels));
            int texture_index = col + row * m_cols;

            m_textures[texture_index].push_back(p_data[pixel_index]);
        }
    }
}

/* ~BlockAtlas
 */
BlockAtlas::~BlockAtlas() {
    free_image_data(p_data);
}

/* get_texture_data
 */
unsigned char *BlockAtlas::get_texture_data(int index) {
    BlockAtlasMapIterator it = m_textures.find(index);
    if (it != m_textures.end()) {
        return it->second.data();
    }

    return nullptr;
}

/* get_num_channels
 */
int BlockAtlas::get_num_channels() const {
    return m_num_channels;
}