#pragma once

namespace Vision {
class VertexBuffer {
  private:
    unsigned int rendererID;

  public:
    VertexBuffer(const void* data, unsigned int size);
    ~VertexBuffer();

    void Bind() const;
    void Unbind() const;
};
} // namespace Vision
