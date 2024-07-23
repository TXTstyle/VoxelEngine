#include "VertexBuffer.hpp"
#include <glad/glad.h>
#include <iostream>

using namespace Vision;

VertexBuffer::VertexBuffer(const void* data, unsigned int size) {
    glGenBuffers(1, &rendererID);
    glBindBuffer(GL_ARRAY_BUFFER, rendererID);
    glBufferData(GL_ARRAY_BUFFER, size, data, GL_DYNAMIC_DRAW);
    std::cout << "Buffer created, id: " << rendererID << std::endl;
}

VertexBuffer::~VertexBuffer() {
    glDeleteBuffers(1, &rendererID);
    std::cout << "Buffer deleted, id: " << rendererID << std::endl;
}

void VertexBuffer::SubData(uint32_t offset, uint32_t size, const void* data) {
    Bind();
    glBufferSubData(GL_ARRAY_BUFFER, offset, size, data);
}

void VertexBuffer::Bind() const { glBindBuffer(GL_ARRAY_BUFFER, rendererID); }

void VertexBuffer::Unbind() const { glBindBuffer(GL_ARRAY_BUFFER, 0); }
