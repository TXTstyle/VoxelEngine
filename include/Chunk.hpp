#pragma once

#include "Renderer.hpp"
#include "VertexArray.hpp"
#include "VertexBuffer.hpp"
#include <array>
#include <cmath>
#include <glm/ext/vector_float4.hpp>
#include <vector>

namespace Voxel {

struct Vertex {
    glm::vec3 pos;
    glm::vec2 uv;
};

class Chunk {
  public:
    Chunk(glm::vec3 worldPos, const std::array<Vertex, 4>& verts);
    ~Chunk();

    void Draw(Vision::Renderer& renderer, Vision::Shader& shader);
    void Build();

    inline size_t GetInstCount() { return sides.size(); }
    inline glm::vec3& GetWorldPos() { return worldPos; }
    inline void SetVoxel(char state,int x, int y, int z) { data[x][y][z] = state; }

  private:
    glm::vec3 worldPos;
    std::array<std::array<std::array<char, 32>, 32>, 32> data;
    std::vector<unsigned int> sides;
    Vision::VertexArray va;
    Vision::VertexBuffer vb;
    Vision::VertexBuffer ivb;

    void AddSides(int x, int y, int z);
    char At(glm::vec3 pos);
};

enum VoxelSides {
    yp = 0,
    yn,
    xp,
    xn,
    zp,
    zn,
};
} // namespace Voxel
