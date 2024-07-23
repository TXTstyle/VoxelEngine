#pragma once

#include "Renderer.hpp"
#include "Chunk.hpp"
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

  private:
    std::unordered_map<glm::ivec3, Chunk> chunks;
    Chunk dummy;
    uint32_t renderDistLen;

    Vision::VertexArray va;
    Vision::VertexBuffer vb;
    Vision::VertexBuffer ivb;
    static const std::array<Voxel::Vertex, 4> verts;


    void BuildChunk(std::pair<const glm::ivec3, Chunk>& chunk);
    char At(std::pair<const glm::ivec3, Chunk>& chunk, glm::vec3 pos);
    Chunk& GetChunkAt(glm::vec3& cPos, const glm::vec3 offset);
};
} // namespace Voxel
