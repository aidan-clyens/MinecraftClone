#include "World.h"


/* World
 */
World::World(Engine *parent):
p_parent(parent),
p_shader_manager(ShaderManager::get_instance()),
p_texture_manager(TextureManager::get_instance())
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
            m_instanced_objects[type] = new Object3D(vec3(0, 0, 0), vec3(0, 0, 0), vec3(1, 1, 1));

            CubeMesh mesh;
            mesh.set_texture(p_texture_manager->get_texture(type));
            mesh.set_light(p_shader_manager->get_light());

            m_instanced_objects[type]->add_component(COMP_MESH, new MeshInstances(&mesh));

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

            MeshInstances *mesh = (MeshInstances*)m_instanced_objects[type]->get_component(COMP_MESH);
            mesh->add_transform(transform);
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