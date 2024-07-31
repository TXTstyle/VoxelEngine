#pragma once

#include "Renderer.hpp"
#include "Chunk.hpp"
#include <glm/ext/vector_int3.hpp>
#include <vector>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/hash.hpp>
#include <unordered_map>
#include <utility>

namespace Voxel {
class Manager {
  public:
    Manager(uint32_t renderDistance, size_t seed);
    ~Manager();

    void Draw(Vision::Renderer& renderer, Vision::Shader& shader);
    void Build();
    void Load(glm::ivec2 pos);

  private:
    std::unordered_map<glm::ivec2, Chunk> chunks;
    std::vector<glm::ivec2> loadChunkList;
    std::vector<glm::ivec2> unLoadChunkList;

    Chunk dummy;
    int renderDistance;
    bool shouldRebuild = true;
    glm::ivec2 chunkPos = {256, 256};

    Vision::VertexArray va;
    Vision::VertexBuffer vb;
    Vision::VertexBuffer ivb;
    static const std::array<Voxel::Vertex, 4> verts;

    void LoadChunk(const glm::ivec2 pos);
    void UnLoadChunk(const glm::ivec2 pos);
    bool IsChunkLoaded(const glm::ivec2& pos);
    void BuildChunk(std::pair<const glm::ivec2, Chunk>& chunk);
    char At(std::pair<const glm::ivec2, Chunk>& chunk, glm::ivec3 pos);
    Chunk& GetChunkAt(glm::ivec2& cPos, const glm::ivec2 offset);
};
} // namespace Voxel
