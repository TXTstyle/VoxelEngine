#include "Chunk.hpp"
#include <noise/PerlinNoise.hpp>
#include <algorithm>
#include <array>

using namespace Voxel;

#include <iostream>

siv::PerlinNoise Chunk::perlin;
siv::PerlinNoise::seed_type Chunk::seed;

void Chunk::SetSeed(siv::PerlinNoise::seed_type newSeed) {
    seed = newSeed;
    perlin.reseed(seed);
}

Chunk::Chunk() : data({}) {
    // std::cout << "Chunk created" << std::endl;
}

Chunk::~Chunk() {
    // std::cout << "Chunk deleted" << std::endl;
}

unsigned int encodeSideData(int x, int y, int z, VoxelSide side, char type) {
    unsigned int data = 0;
    data = static_cast<unsigned int>(type);
    data = (data << 3) | static_cast<unsigned int>(side);
    data = (data << 4) | x;
    data = (data << 8) | y;
    data = (data << 4) | z;

    return data;
}

void Chunk::AddSide(int x, int y, int z, VoxelSide side, char voxel) {
    sides.push_back(encodeSideData(x, y, z, side, voxel));
}

char Chunk::At(glm::vec3 pos) { return data[pos.x][pos.y][pos.z]; }

void Chunk::Fill(const char voxel) {
    std::fill_n(&data[0][0][0], 16 * 256 * 16, voxel);
}

void Chunk::Gen(glm::ivec2 offset) {
    if (isBuilt)
        return;

    for (int y = 0; y < 256; ++y) {
        for (int x = 0; x < 16; ++x) {
            for (int z = 0; z < 16; z++) {
                char voxel = 0;
                const double scale = 0.005;
                const double noise =
                    perlin.octave2D_01((x + offset.x * 16) * scale,
                                       (z + offset.y * 16) * scale, 6);

                if (noise * 128 > y) {
                    voxel = 1;
                }

                SetVoxel(voxel, x, y, z);
            }
        }
    }
    std::cout << "Chunk generated, at: " << offset.x << " " << offset.y << std::endl;
    isBuilt = true;
}
