#include "ChunkManager.hpp"
#include "Chunk.hpp"
#include <cmath>
#include <iostream>

using namespace Voxel;

const std::array<Voxel::Vertex, 4> Manager::verts = {
    Voxel::Vertex{{-0.5f, -0.5f, -0.5f}, {0.0f, 0.0f}},
    Voxel::Vertex{{-0.5f, -0.5f, 0.5f}, {0.0f, 1.0f}},
    Voxel::Vertex{{0.5f, -0.5f, -0.5f}, {1.0f, 0.0f}},
    Voxel::Vertex{{0.5f, -0.5f, 0.5f}, {1.0f, 1.0f}},
};

//                               Size of Chunk * Sides * Size of Face
const uint32_t CHUNK_SIZE = 32 * 32 * 32 * 6 * sizeof(unsigned int);

Manager::Manager(uint32_t renderDistance)
    : vb(verts.data(), verts.size() * sizeof(Voxel::Vertex)),
      ivb(nullptr, CHUNK_SIZE * std::pow(renderDistance * 2 + 1, 3)) {
    va.Bind();
    vb.Bind();

    Vision::VertexBufferLayout layout;
    layout.Push<float>(3);
    layout.Push<float>(2);
    va.AddBuffer(vb, layout);

    glEnableVertexAttribArray(2);
    ivb.Bind();
    glVertexAttribIPointer(2, 1, GL_UNSIGNED_INT, sizeof(unsigned int),
                           (void*)0);
    ivb.Unbind();
    glVertexAttribDivisor(2, 1);

    chunks.reserve(std::pow(renderDistance * 2 + 1, 3));
    for (uint32_t x = 0; x < (renderDistance * 2 + 1); x++) {
        for (uint32_t y = 0; y < (renderDistance * 2 + 1); y++) {
            for (uint32_t z = 0; z < (renderDistance * 2 + 1); z++) {
                chunks.emplace_back(glm::vec3(x * 32, y * 32, z * 32));
            }
        }
    }

    std::cout << "Manager created, chunk count: "
              << std::pow(renderDistance * 2 + 1, 3) << std::endl;
}

Manager::~Manager() { std::cout << "Manager destroyed." << std::endl; }

void Manager::Draw(Vision::Renderer& renderer, Vision::Shader& shader) {
    size_t offset = 0;
    for (uint32_t i = 0; i < chunks.size(); i++) {
        auto& chunk = chunks[i];
        va.Bind();
        shader.Use();
        shader.SetVec3f("u_WorldPos", chunk.GetWorldPos());
        renderer.Draw(va, shader, chunk.GetInstCount(), offset);
        offset += chunk.GetInstCount();
    }
}

void Manager::Build() {
    size_t offset = 0;
    for (int i = 0; i < chunks.size(); i++) {
        BuildChunk(chunks[i], i);
        ivb.Bind();
        ivb.SubData(offset,
                    chunks[i].GetInstCount() * sizeof(unsigned int),
                    chunks[i].GetSides());
        offset += chunks[i].GetInstCount() * sizeof(unsigned int);
    }
}

void Manager::BuildChunk(Chunk& chunk, int index) {
    chunk.ClearSides();
    for (int x = 0; x < 32; x++) {
        for (int y = 0; y < 32; y++) {
            for (int z = 0; z < 32; z++) {
                // if empty exit
                char voxel = At(chunk, {x, y, z});
                if (!voxel)
                    return;

                // Check faces
                if (!At(chunk, {x, y + 1, z}, index))
                    chunk.AddSide(x, y, z, VoxelSide::yp, voxel);
                if (!At(chunk, {x, y - 1, z}, index))
                    chunk.AddSide(x, y, z, VoxelSide::yn, voxel);
                if (!At(chunk, {x + 1, y, z}, index))
                    chunk.AddSide(x, y, z, VoxelSide::xp, voxel);
                if (!At(chunk, {x - 1, y, z}, index))
                    chunk.AddSide(x, y, z, VoxelSide::xn, voxel);
                if (!At(chunk, {x, y, z + 1}, index))
                    chunk.AddSide(x, y, z, VoxelSide::zp, voxel);
                if (!At(chunk, {x, y, z - 1}, index))
                    chunk.AddSide(x, y, z, VoxelSide::zn, voxel);
            }
        }
    }
}

char Manager::At(Chunk& chunk, glm::vec3 pos, int index) {
    if (pos.x < 0 || pos.y < 0 || pos.z < 0) {
        return 0;
    }

    if (pos.x > 31 || pos.y > 31 || pos.z > 31) {
        // std::cout << "Overflow at: " << pos.x << " " << pos.y << " " << pos.z
        //           << std::endl;
        return 0;
    }

    return chunk.At(pos);
}
