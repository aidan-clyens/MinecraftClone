#pragma once

// Includes
#include "Engine/utils/load_image.h"

#include <string>
#include <vector>
#include <unordered_map>

// Typedefs
typedef std::unordered_map<int, std::vector<unsigned char>> BlockAtlasMap;
typedef std::unordered_map<int, std::vector<unsigned char>>::iterator BlockAtlasMapIterator;

/* BlockAtlas
 */
class BlockAtlas {
    public:
        BlockAtlas(const std::string &path, int cols, int rows, int texture_width);
        virtual ~BlockAtlas();

        unsigned char *get_texture_data(int index);

        int get_num_channels() const;

    private:
        int m_cols;
        int m_rows;
        int m_texture_width;

        int m_atlas_width;
        int m_atlas_height;
        int m_num_channels;

        unsigned char *p_data;
        BlockAtlasMap m_textures;
};