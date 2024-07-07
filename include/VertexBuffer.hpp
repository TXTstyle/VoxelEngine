#pragma once

#include <cstdint>

namespace Vision {
class VertexBuffer {
  private:
    unsigned int rendererID;

  public:
    VertexBuffer(const void* data, unsigned int size);
    ~VertexBuffer();

    void SubData(uint32_t offset, uint32_t size, const void* data);

    void Bind() const;
    void Unbind() const;
};
} // namespace Vision
