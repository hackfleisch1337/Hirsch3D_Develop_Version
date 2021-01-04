#include "VertexBuffer.hpp"


h3d::VertexBuffer::VertexBuffer(void* data, uint32_t amountOfVertices) {

    this->amountOfVertices = amountOfVertices;

    glGenVertexArrays(1, &this->vao);
    glBindVertexArray(this->vao);

    glGenBuffers(1, &this->bufferId);
    glBindBuffer(GL_ARRAY_BUFFER, this->bufferId);
    glBufferData(GL_ARRAY_BUFFER, amountOfVertices * sizeof(h3d::Vertex3), data, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(h3d::Vertex3), static_cast<void*>(offsetof(struct h3d::Vertex3, x)));
    glBindVertexArray(0);
}

h3d::VertexBuffer::~VertexBuffer() {
    glDeleteBuffers(1, &this->bufferId);
    std::cout << "Deleted Bufferdata from VRAM" << std::endl;
}

void h3d::VertexBuffer::bind() {
    glBindVertexArray(this->vao);
}

void h3d::VertexBuffer::unbind() {
    glBindVertexArray(0);
}

uint32_t h3d::VertexBuffer::getAmountOfVertices() {
    return this->amountOfVertices;
}