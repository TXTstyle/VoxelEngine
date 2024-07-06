#pragma once

#include "VertexBuffer.hpp"
#include "VertexBufferLayout.hpp"

namespace Vision {
class VertexArray {
  private:
    unsigned int rendererID;

  public:
    VertexArray();
    ~VertexArray();

    void AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout);

    void Bind() const;
    void Unbind() const;
};
} // namespace Vision
