#include "VertexBuffer.hpp"
#include <glad/glad.h>

using namespace Vision;

VertexBuffer::VertexBuffer(const void* data, unsigned int size) {
    glGenBuffers(1, &rendererID);
    glBindBuffer(GL_ARRAY_BUFFER, rendererID);
    glBufferData(GL_ARRAY_BUFFER, size, data, GL_DYNAMIC_DRAW);
}

VertexBuffer::~VertexBuffer() { glDeleteBuffers(1, &rendererID); }

void VertexBuffer::SubData(uint32_t offset, uint32_t size, const void* data) {
    Bind();
    glBufferSubData(GL_ARRAY_BUFFER, offset, size, data);
}

void VertexBuffer::Bind() const { glBindBuffer(GL_ARRAY_BUFFER, rendererID); }

void VertexBuffer::Unbind() const { glBindBuffer(GL_ARRAY_BUFFER, 0); }
