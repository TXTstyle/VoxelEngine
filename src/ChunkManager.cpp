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

//                        Size of Chunk * Sides * Size of Face
const uint32_t CHUNK_SIZE = 32 * 32 * 32 * 6 * sizeof(unsigned int);

Manager::Manager(uint32_t renderDistance)
    : vb(verts.data(), verts.size() * sizeof(Voxel::Vertex)),
      renderDistLen(renderDistance * 2 + 1), dummy({0, 0, 0}),
      ivb(nullptr, CHUNK_SIZE * std::pow(renderDistLen, 3)) {
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

    dummy.Fill(0);

    chunks.reserve(std::pow(renderDistLen, 3));
    for (uint32_t x = 0; x < renderDistLen; x++) {
        for (uint32_t y = 0; y < renderDistLen; y++) {
            for (uint32_t z = 0; z < renderDistLen; z++) {
                chunks.emplace_back(glm::vec3(x, y, z));
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
        shader.SetVec3f("u_WorldPos", chunk.GetWorldPos() * 32.0f);
        renderer.Draw(va, shader, chunk.GetInstCount(), offset);
        offset += chunk.GetInstCount();
    }
}

void Manager::Build() {
    size_t offset = 0;
    for (int i = 0; i < chunks.size(); i++) {
        BuildChunk(chunks[i]);
        ivb.Bind();
        ivb.SubData(offset, chunks[i].GetInstCount() * sizeof(unsigned int),
                    chunks[i].GetSides());
        offset += chunks[i].GetInstCount() * sizeof(unsigned int);
    }
}

void Manager::BuildChunk(Chunk& chunk) {
    chunk.ClearSides();
    for (int x = 0; x < 32; x++) {
        for (int y = 0; y < 32; y++) {
            for (int z = 0; z < 32; z++) {
                char voxel = chunk.At({x, y, z});

                // if empty exit
                if (!voxel)
                    return;

                // Check faces
                if (!At(chunk, {x, y + 1, z}))
                    chunk.AddSide(x, y, z, VoxelSide::yp, voxel);
                if (!At(chunk, {x, y - 1, z}))
                    chunk.AddSide(x, y, z, VoxelSide::yn, voxel);
                if (!At(chunk, {x + 1, y, z}))
                    chunk.AddSide(x, y, z, VoxelSide::xp, voxel);
                if (!At(chunk, {x - 1, y, z}))
                    chunk.AddSide(x, y, z, VoxelSide::xn, voxel);
                if (!At(chunk, {x, y, z + 1}))
                    chunk.AddSide(x, y, z, VoxelSide::zp, voxel);
                if (!At(chunk, {x, y, z - 1}))
                    chunk.AddSide(x, y, z, VoxelSide::zn, voxel);
            }
        }
    }
    std::cout << "Chunk built, sides: " << chunk.GetInstCount() << std::endl;
}

Chunk& Manager::GetChunkAt(glm::vec3& cPos, const glm::vec3 offset) {
    int x = cPos.x + offset.x;
    int y = cPos.y + offset.y;
    int z = cPos.z + offset.z;

    if (x < 0 || y < 0 || z < 0) {
        return dummy; // Remember THIS!
    }
    if (x >= renderDistLen || y >= renderDistLen || z >= renderDistLen) {
        return dummy; // Remember THIS!
    }

    int index = x + renderDistLen * (y + renderDistLen * z);

    return chunks[index];
}

char Manager::At(Chunk& chunk, glm::vec3 pos) {
    glm::vec3 cPos = chunk.GetWorldPos();

    if (pos.x == -1) {
        return GetChunkAt(cPos, {-1, 0, 0}).At({31, pos.y, pos.z});
    } else if (pos.y == -1) {
        return GetChunkAt(cPos, {0, -1, 0}).At({pos.x, 31, pos.z});
    } else if (pos.z == -1) {
        return GetChunkAt(cPos, {0, 0, -1}).At({pos.x, pos.y, 31});
    }

    if (pos.x == 32) {
        return GetChunkAt(cPos, {1, 0, 0}).At({0, pos.y, pos.z});
    } else if (pos.y == 32) {
        return GetChunkAt(cPos, {0, 1, 0}).At({pos.x, 0, pos.z});
    } else if (pos.z == 32) {
        return GetChunkAt(cPos, {0, 0, 1}).At({pos.x, pos.y, 0});
    }

    return chunk.At(pos);
}
