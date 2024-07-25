#include "ChunkManager.hpp"
#include "Chunk.hpp"
#include <cmath>
#include <glm/ext/vector_int3.hpp>
#include <iostream>
#include <stdexcept>
#include <unordered_map>
#include <utility>
#include <glm/gtx/string_cast.hpp>

using namespace Voxel;

const std::array<Voxel::Vertex, 4> Manager::verts = {
    Voxel::Vertex{{-0.5f, -0.5f, -0.5f}, {0.0f, 0.0f}},
    Voxel::Vertex{{-0.5f, -0.5f, 0.5f}, {0.0f, 1.0f}},
    Voxel::Vertex{{0.5f, -0.5f, -0.5f}, {1.0f, 0.0f}},
    Voxel::Vertex{{0.5f, -0.5f, 0.5f}, {1.0f, 1.0f}},
};

//                        Size of Chunk * Sides * Size of Face
const uint32_t CHUNK_SIZE = 32 * 32 * 32 * 6 * sizeof(unsigned int);

Manager::Manager(uint32_t renderDistance, size_t seed)
    : chunks(), dummy(), renderDistance(renderDistance),
      vb(verts.data(), verts.size() * sizeof(Voxel::Vertex)),
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

    Chunk::SetSeed(seed);
    dummy.Fill(0);

    chunks.reserve(std::pow(renderDistance * 2 + 1, 3));

    std::cout << "Manager created, chunk count: " << chunks.size() << std::endl;
}

Manager::~Manager() { std::cout << "Manager destroyed." << std::endl; }

void Manager::Load(glm::ivec3 pos) {
    if (pos == chunkPos)
        return;

    chunks.clear();

    LoadChunk(pos);
    for (int x = -renderDistance; x <= renderDistance; x++) {
        for (int y = -renderDistance; y <= renderDistance; y++) {
            for (int z = -renderDistance; z <= renderDistance; z++) {
                glm::ivec3 offset = {x, y, z};
                LoadChunk(pos + offset);
            }
        }
    }
    chunkPos = pos;
    shouldRebuild = true;
}

void Manager::Draw(Vision::Renderer& renderer, Vision::Shader& shader) {
    size_t offset = 0;
    for (auto& chunk : chunks) {
        va.Bind();
        shader.Use();
        shader.SetVec3f("u_WorldPos", chunk.first);
        renderer.Draw(va, shader, chunk.second.GetInstCount(), offset);
        offset += chunk.second.GetInstCount();
    }
}

void Manager::Build() {
    if (!shouldRebuild)
        return;

    size_t offset = 0;
    for (auto& chunk : chunks) {
        chunk.second.Gen(chunk.first);
        BuildChunk(chunk);
        ivb.Bind();
        ivb.SubData(offset, chunk.second.GetInstCount() * sizeof(unsigned int),
                    chunk.second.GetSides());
        offset += chunk.second.GetInstCount() * sizeof(unsigned int);
    }
    std::cout << "Building completed, buffer size: " << offset
              << ", chunks: " << chunks.size() << std::endl;
    shouldRebuild = false; // Remember!!!
}

void Manager::BuildChunk(std::pair<const glm::ivec3, Chunk>& chunk) {
    chunk.second.ClearSides();
    for (int x = 0; x < 32; x++) {
        for (int y = 0; y < 32; y++) {
            for (int z = 0; z < 32; z++) {
                char voxel = chunk.second.At({x, y, z});

                // if empty continue
                if (!voxel)
                    continue;

                // Check faces
                if (!At(chunk, {x, y + 1, z}))
                    chunk.second.AddSide(x, y, z, VoxelSide::yp, voxel);
                if (!At(chunk, {x, y - 1, z}))
                    chunk.second.AddSide(x, y, z, VoxelSide::yn, voxel);
                if (!At(chunk, {x + 1, y, z}))
                    chunk.second.AddSide(x, y, z, VoxelSide::xp, voxel);
                if (!At(chunk, {x - 1, y, z}))
                    chunk.second.AddSide(x, y, z, VoxelSide::xn, voxel);
                if (!At(chunk, {x, y, z + 1}))
                    chunk.second.AddSide(x, y, z, VoxelSide::zp, voxel);
                if (!At(chunk, {x, y, z - 1}))
                    chunk.second.AddSide(x, y, z, VoxelSide::zn, voxel);
            }
        }
    }
    std::cout << "Chunk built, sides: " << chunk.second.GetInstCount()
              << std::endl;
}

Chunk& Manager::GetChunkAt(glm::vec3& cPos, const glm::vec3 offset) {
    int x = cPos.x + offset.x;
    int y = cPos.y + offset.y;
    int z = cPos.z + offset.z;

    try {
        return chunks.at({x, y, z});
    } catch (const std::out_of_range& e) {
        return dummy;
    }
}

char Manager::At(std::pair<const glm::ivec3, Chunk>& chunk, glm::vec3 pos) {
    glm::vec3 cPos = chunk.first;

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

    return chunk.second.At(pos);
}

void Manager::LoadChunk(const glm::ivec3 pos) {
    if (IsChunkLoaded(pos))
        return;

    chunks[pos] = Chunk();
    std::cout << "Chunk loaded, at: " << pos.x << " " << pos.y << " " << pos.z
              << std::endl;
}

void Manager::UnLoadChunk(const glm::ivec3 pos) {
    if (!IsChunkLoaded(pos))
        return;

    chunks.erase(pos);
}

bool Manager::IsChunkLoaded(const glm::ivec3& pos) {
    try {
        chunks.at(pos);
    } catch (const std::out_of_range& e) {
        return false;
    }
    return true;
}
