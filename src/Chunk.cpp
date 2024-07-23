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
    data = (data << 5) | x;
    data = (data << 5) | y;
    data = (data << 5) | z;

    return data;
}

void Chunk::AddSide(int x, int y, int z, VoxelSide side, char voxel) {
    sides.push_back(encodeSideData(x, y, z, side, voxel));
}

char Chunk::At(glm::vec3 pos) { return data[pos.x][pos.y][pos.z]; }

void Chunk::Fill(const char voxel) {
    std::fill_n(&data[0][0][0], 32 * 32 * 32, voxel);
}

void Chunk::Gen(glm::ivec3 offset) {
    for (int y = 0; y < 32; ++y) {
        for (int x = 0; x < 32; ++x) {
            for (int z = 0; z < 32; z++) {
                char voxel = 0;
                const double scale = 0.015;
                const double noise =
                    perlin.octave3D_01((x + offset.x * 32) * scale,
                                       (y + offset.y * 32) * scale,
                                       (z + offset.z * 32) * scale, 6);

                if (noise > 0.5) {
                    voxel = 3;
                }

                SetVoxel(voxel, x, y, z);
            }
        }
    }
    std::cout << "Chunk generated, at: " << offset.x << " " << offset.y << " "
              << offset.z << std::endl;
}
