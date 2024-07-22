#pragma once

#include "Renderer.hpp"
#include "Chunk.hpp"
#include <vector>

namespace Voxel {
class Manager {
  public:
    Manager(uint32_t renderDistance);
    ~Manager();

    void Draw(Vision::Renderer& renderer, Vision::Shader& shader);
    void Build();

  private:
    std::vector<Voxel::Chunk> chunks;
    Chunk dummy;
    uint32_t renderDistLen;

    Vision::VertexArray va;
    Vision::VertexBuffer vb;
    Vision::VertexBuffer ivb;
    static const std::array<Voxel::Vertex, 4> verts;

    void BuildChunk(Chunk& chunk);
    char At(Chunk& chunk, glm::vec3 pos);
    Chunk& GetChunkAt(glm::vec3& cPos, const glm::vec3 offset);
};
} // namespace Voxel
