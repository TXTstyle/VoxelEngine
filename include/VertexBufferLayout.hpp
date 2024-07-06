#pragma once
#include <vector>
#include <cassert>
#include <glad/glad.h>

namespace Vision {

struct VertexBufferElement {
    unsigned int type;
    unsigned int count;
    unsigned char normalized;

    static unsigned int GetSizeOfType(uint32_t type) {
        switch (type) {
        case GL_FLOAT:
            return 4;
        case GL_UNSIGNED_INT:
            return 4;
        case GL_UNSIGNED_BYTE:
            return 1;
        }
        assert(false);
        return 0;
    }
};

class VertexBufferLayout {
  private:
    std::vector<VertexBufferElement> m_Elements;
    unsigned int m_Stride;

  public:
    VertexBufferLayout() : m_Stride(0) {}

    template <typename T> void Push(uint32_t count) { static_assert(false); }

    inline const std::vector<VertexBufferElement> GetElements() const& {
        return m_Elements;
    }
    inline unsigned int GetStride() const { return m_Stride; }
};

template <> inline void VertexBufferLayout::Push<float>(uint32_t count) {
    m_Elements.push_back({GL_FLOAT, count, GL_FALSE});
    m_Stride += VertexBufferElement::GetSizeOfType(GL_FLOAT) * count;
}

template <> inline void VertexBufferLayout::Push<uint32_t>(uint32_t count) {
    m_Elements.push_back({GL_UNSIGNED_INT, count, GL_FALSE});
    m_Stride += VertexBufferElement::GetSizeOfType(GL_UNSIGNED_INT) * count;
}

template <>
inline void VertexBufferLayout::Push<unsigned char>(uint32_t count) {
    m_Elements.push_back({GL_UNSIGNED_BYTE, count, GL_TRUE});
    m_Stride += VertexBufferElement::GetSizeOfType(GL_UNSIGNED_BYTE) * count;
}
} // namespace Vision
