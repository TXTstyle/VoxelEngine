#include "VertexArray.hpp"
#include "VertexBufferLayout.hpp"

using namespace Vision;

#include <iostream>

VertexArray::VertexArray() {
    glGenVertexArrays(1, &rendererID);
    Bind();
    std::cout << "VAO created, id: " << rendererID << std::endl;
}

VertexArray::~VertexArray() { 
    glDeleteVertexArrays(1, &rendererID);
    std::cout << "VAO deleted, id: " << rendererID << std::endl;

}

void VertexArray::AddBuffer(const VertexBuffer& vb,
                            const VertexBufferLayout& layout) {
    Bind();
    vb.Bind();
    const auto& elements = layout.GetElements();
    unsigned int offset = 0;
    for (uint32_t i = 0; i < elements.size(); i++) {
        const auto& element = elements[i];
        glEnableVertexAttribArray(i);
        glVertexAttribPointer(i, element.count, element.type,
                              element.normalized, layout.GetStride(),
                              (const void*)offset);
        offset +=
            element.count * VertexBufferElement::GetSizeOfType(element.type);
    }
}

void VertexArray::Bind() const { glBindVertexArray(rendererID); }

void VertexArray::Unbind() const { glBindVertexArray(0); }
