#pragma once

#include <iostream>
#include "noise/PerlinNoise.hpp"
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
    Chunk();
    Chunk(Chunk&&) = default;
    Chunk(const Chunk&) = default;
    Chunk& operator=(Chunk&&) = default;
    Chunk& operator=(const Chunk&) = default;
    ~Chunk();

    void AddSide(int x, int y, int z, VoxelSide side, char voxel);
    char At(glm::vec3 pos);

    void Fill(const char voxel);
    void Gen(glm::ivec3 offset);

    inline size_t GetInstCount() { return sides.size(); }
    inline void SetVoxel(char voxel, unsigned int x, unsigned int y,
                         unsigned int z) {
        data[x][y][z] = voxel;
    }
    inline void ClearSides() { sides.clear(); };
    inline unsigned int* GetSides() { return sides.data(); }

    static void SetSeed(siv::PerlinNoise::seed_type newSeed);
    inline static siv::PerlinNoise::seed_type GetSeed() { return seed; };

  private:
    std::array<std::array<std::array<char, 32>, 32>, 32> data;
    std::vector<unsigned int> sides;
    bool isBuilt = false;

    static siv::PerlinNoise::seed_type seed;
    static siv::PerlinNoise perlin;
};
} // namespace Voxel
