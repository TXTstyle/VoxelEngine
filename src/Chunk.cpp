#include "Chunk.hpp"
#include "Shader.hpp"
#include "VertexArray.hpp"
#include <array>

using namespace Voxel;

#include <iostream>

Chunk::Chunk(glm::vec3 worldPos, const std::array<Vertex, 4>& verts)
    : worldPos(worldPos),
      vb(verts.data(), verts.size() * sizeof(Voxel::Vertex)),
      ivb(nullptr, 32 * 32 * 32 * 6 * sizeof(glm::vec4)) {
    va.Bind();
    vb.Bind();

    Vision::VertexBufferLayout layout;
    layout.Push<float>(3);
    layout.Push<float>(2);
    va.AddBuffer(vb, layout);

    glEnableVertexAttribArray(2);
    ivb.Bind();
    glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(glm::vec4),
                          (void*)0);
    ivb.Unbind();
    glVertexAttribDivisor(2, 1);

    for (int x = 0; x < 32; x++) {
        for (int y = 0; y < 32; y++) {
            for (int z = 0; z < 32; z++) {
                data[x][y][z] = true;
            }
        }
    }

    std::cout << "Chunk created, at: " << this->worldPos.x << " "
              << this->worldPos.y << " " << this->worldPos.z << std::endl;
}

Chunk::~Chunk() { std::cout << "Chunk deleted" << std::endl; }

void Chunk::Draw(Vision::Renderer& renderer, Vision::Shader& shader) {
    va.Bind();
    shader.Use();
    shader.SetVec3f("u_WorldPos", worldPos);
    renderer.Draw(va, shader, sides.size());
}

void Chunk::Build() {
    sides.clear();
    for (int x = 0; x < 32; x++) {
        for (int y = 0; y < 32; y++) {
            for (int z = 0; z < 32; z++) {
                AddSides(x, y, z);
            }
        }
    }
    ivb.Bind();
    ivb.SubData(0, sides.size() * sizeof(glm::vec4), sides.data());
    std::cout << "Chunk Built, sides: " << sides.size() << std::endl;
}

void Chunk::AddSides(int x, int y, int z) {
    bool voxel = At({x, y, z});

    // if empty exit
    if (!voxel)
        return;

    // Check faces
    if (!At({x, y + 1, z}))
        sides.push_back({x, y, z, (float)VoxelSides::yp});
    if (!At({x, y - 1, z}))
        sides.push_back({x, y, z, (float)VoxelSides::yn});
    if (!At({x + 1, y, z}))
        sides.push_back({x, y, z, (float)VoxelSides::xp});
    if (!At({x - 1, y, z}))
        sides.push_back({x, y, z, (float)VoxelSides::xn});
    if (!At({x, y, z + 1}))
        sides.push_back({x, y, z, (float)VoxelSides::zp});
    if (!At({x, y, z - 1}))
        sides.push_back({x, y, z, (float)VoxelSides::zn});
}

bool Chunk::At(glm::vec3 pos) {
    if (pos.x < 0 || pos.y < 0 || pos.z < 0) {
        // std::cout << "Underflow at: " << pos.x << " " << pos.y << " " << pos.z
        //           << std::endl;
        return false;
    }

    if (pos.x > 31 || pos.y > 31 || pos.z > 31) {
        // std::cout << "Overflow at: " << pos.x << " " << pos.y << " " << pos.z
        //           << std::endl;
        return false;
    }

    return data[pos.x][pos.y][pos.z];
}
