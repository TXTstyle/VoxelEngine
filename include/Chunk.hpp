#pragma once

#include <glm/glm.hpp>
#include <array>
#include <cmath>
#include <glm/ext/vector_float4.hpp>
#include <vector>

namespace Voxel {

struct Vertex {
    glm::vec3 pos;
    glm::vec2 uv;
};

enum VoxelSide {
    yp = 0,
    yn,
    xp,
    xn,
    zp,
    zn,
};

class Chunk {
  public:
    Chunk(glm::vec3 worldPos);
    ~Chunk();

    void AddSide(int x, int y, int z, VoxelSide side, char voxel);
    char At(glm::vec3 pos);

    void Fill(char voxel);

    inline size_t GetInstCount() { return sides.size(); }
    inline glm::vec3& GetWorldPos() { return worldPos; }
    inline void SetVoxel(char state, int x, int y, int z) {
        data[x][y][z] = state;
    }
    inline void ClearSides() { sides.clear(); };
    inline unsigned int* GetSides() { return sides.data(); }

  private:
    glm::vec3 worldPos;
    std::array<std::array<std::array<char, 32>, 32>, 32> data;
    std::vector<unsigned int> sides;
};
} // namespace Voxel
