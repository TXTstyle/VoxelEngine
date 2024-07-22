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

    Vision::VertexArray va;
    Vision::VertexBuffer vb;
    Vision::VertexBuffer ivb;
    static const std::array<Voxel::Vertex, 4> verts;

    void BuildChunk(Chunk& chunk, int index);
    char At(Chunk& chunk, glm::vec3 pos, int index = 0);
};
} // namespace Voxel
