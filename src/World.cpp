#include "World.h"


/* World
 */
World::World(Engine *parent):
p_parent(parent)
{
    HeightMapGenerator::init();
}

/* ~World
 */
World::~World() {
    for (ChunkMapIterator it = m_chunks.begin(); it != m_chunks.end(); it++) {
        delete it->second;
    }
}

/* add_chunk
 */
void World::add_chunk(Chunk *chunk) {
    BlockTypeMap *blocks = chunk->get_block_map();

    for (BlockTypeMapIterator it = blocks->begin(); it != blocks->end(); it++) {
        vec3 block_position = it->first;
        eBlockType type = it->second;

        m_world_blocks[block_position] = type;

        if (m_instanced_objects.find(type) == m_instanced_objects.end()) {
            Block *block = new Block(type, vec3(0, 0, 0));

            m_instanced_objects[type] = new Object3DGroup(block);
            p_parent->add_object(m_instanced_objects[type]);
        }
    }
}

/* update_world_blocks
 */
void World::update_world_blocks() {
    for (BlockTypeMapIterator it = m_world_blocks.begin(); it != m_world_blocks.end(); it++) {
        vec3 block_position = it->first;
        eBlockType type = it->second;

        // Determine if block is visible
        if (this->is_block_visible(block_position)) {
            Transform transform;
            transform.position = block_position;
            transform.rotation = vec3(0, 0, 0);
            transform.size = vec3(1, 1, 1);

            m_instanced_objects[type]->add_transform(transform);
        }
    }
}

/* is_block_visible
 */
bool World::is_block_visible(vec3 position) {
    return !(m_world_blocks.find(vec3(position.x + 1, position.y, position.z)) != m_world_blocks.end() && // +x
             m_world_blocks.find(vec3(position.x - 1, position.y, position.z)) != m_world_blocks.end() && // -x
             m_world_blocks.find(vec3(position.x, position.y + 1, position.z)) != m_world_blocks.end() && // +y
             m_world_blocks.find(vec3(position.x, position.y - 1, position.z)) != m_world_blocks.end() && // -y
             m_world_blocks.find(vec3(position.x, position.y, position.z + 1)) != m_world_blocks.end() && // +z
             m_world_blocks.find(vec3(position.x, position.y, position.z - 1)) != m_world_blocks.end());  // -z
}