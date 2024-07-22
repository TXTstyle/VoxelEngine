#include "Chunk.hpp"
#include <array>

using namespace Voxel;

#include <iostream>

Chunk::Chunk(glm::vec3 worldPos) : worldPos(worldPos) {
    int t = 0;
    for (int x = 0; x < 32; x++) {
        for (int y = 0; y < 32; y++) {
            for (int z = 0; z < 32; z++) {
                data[x][y][z] = t % 4 + 1;
                t++;
            }
        }
    }

    std::cout << "Chunk created, at: " << this->worldPos.x << " "
              << this->worldPos.y << " " << this->worldPos.z << std::endl;
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

void Chunk::Fill(char voxel) {
    for (int x = 0; x < 32; x++) {
        for (int y = 0; y < 32; y++) {
            for (int z = 0; z < 32; z++) {
                data[x][y][z] = voxel;
            }
        }
    }
}
