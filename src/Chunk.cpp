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
    std::cout << "Chunk created, at: " << this->worldPos.x << " " << this->worldPos.y << " "
              << this->worldPos.z << std::endl;
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
                AddCude({x, y, z});
            }
        }
    }
    ivb.Bind();
    ivb.SubData(0, sides.size() * sizeof(glm::vec4), sides.data());
    std::cout << "Chunk Built, sides: " << sides.size() << std::endl;
}

void Chunk::AddCude(glm::vec3 pos) {
    sides.push_back({pos.x, pos.y, pos.z, (float)VoxelSides::yp});
    sides.push_back({pos.x, pos.y, pos.z, (float)VoxelSides::yn});
    sides.push_back({pos.x, pos.y, pos.z, (float)VoxelSides::xp});
    sides.push_back({pos.x, pos.y, pos.z, (float)VoxelSides::xn});
    sides.push_back({pos.x, pos.y, pos.z, (float)VoxelSides::zp});
    sides.push_back({pos.x, pos.y, pos.z, (float)VoxelSides::zn});
}
