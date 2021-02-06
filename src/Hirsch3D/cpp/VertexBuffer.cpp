#include "../Include/Hirsch3D/core/VertexBuffer.hpp"
#ifndef GREEN
#define GREEN "\033[92m"
#define RESET_CLR "\x1B[0m"
#endif

h3d::VertexBuffer::VertexBuffer(void* data, uint32_t amountOfVertices) {

    this->amountOfVertices = amountOfVertices;

    glGenVertexArrays(1, &this->vao);
    glBindVertexArray(this->vao);

    glGenBuffers(1, &this->bufferId);
    glBindBuffer(GL_ARRAY_BUFFER, this->bufferId);
    glBufferData(GL_ARRAY_BUFFER, amountOfVertices * sizeof(h3d::Vertex3), data, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(h3d::Vertex3), static_cast<void*>(offsetof(struct h3d::Vertex3, x)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(h3d::Vertex3), (void*) (offsetof(struct h3d::Vertex3, u)));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(h3d::Vertex3), (void*) (offsetof(struct h3d::Vertex3, nx)));
    
    glBindVertexArray(0);
}

h3d::VertexBuffer::~VertexBuffer() {
    glDeleteBuffers(1, &this->bufferId);
    std::cout << GREEN << "[Ok] Deleted Vertex Bufferdata from VRAM" << RESET_CLR << std::endl;
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