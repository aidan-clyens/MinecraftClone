#pragma once

// Includes
#include "Engine/Engine.h"
#include "Engine/Object3DGroup.h"

#include "Terrain/HeightMapGenerator.h"
#include "Block.h"
#include "Chunk.h"

#include <unordered_map>

// Typedefs
typedef std::unordered_map<eBlockType, Object3DGroup*> InstancedObjectMap;
typedef std::unordered_map<eBlockType, Object3DGroup*>::iterator InstancedObjectMapIterator;

typedef std::unordered_map<vec2, Chunk*, vec2_key_hash, vec2_key_equal> ChunkMap;
typedef std::unordered_map<vec2, Chunk*, vec2_key_hash, vec2_key_equal>::iterator ChunkMapIterator;


/* World
 */
class World {
    public:
        World(Engine *parent);
        virtual ~World();

        void add_chunk(Chunk *chunk);
        void update_world_blocks();

        bool is_block_visible(vec3 position);

    private:
        Engine *p_parent;

        InstancedObjectMap m_instanced_objects;
        ChunkMap m_chunks;
        BlockTypeMap m_world_blocks;
};