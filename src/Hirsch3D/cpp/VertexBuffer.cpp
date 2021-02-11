#include "../Include/Hirsch3D/core/VertexBuffer.hpp"
#ifndef GREEN
#define GREEN "\033[92m"
#define YELLOW "\033[93m"
#define RED "\033[91m"
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
    glBindBuffer(GL_ARRAY_BUFFER, 0);
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
    glBindBuffer(GL_VERTEX_ARRAY, 0);
}

uint32_t h3d::VertexBuffer::getAmountOfVertices() {
    return this->amountOfVertices;
}

void h3d::VertexBuffer::changeVertex(uint32_t vertex, h3d::Vertex3 newVertex) {
    if(vertex >= this->amountOfVertices) {
        std::cout << RED << "[FAILED] Vetex number out of bounds!" << RESET_CLR << std::endl;
        return;
    }
    glBindBuffer(GL_ARRAY_BUFFER, this->bufferId);
    glBufferSubData(GL_ARRAY_BUFFER, vertex * sizeof(h3d::Vertex3), sizeof(h3d::Vertex3), (void*) &newVertex);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

h3d::Vertex3 h3d::VertexBuffer::getVertex(uint32_t vertex) {
    h3d::Vertex3 r;

    glBindBuffer(GL_ARRAY_BUFFER, this->bufferId);
    
    glGetBufferSubData(GL_ARRAY_BUFFER, vertex * sizeof(h3d::Vertex3), sizeof(h3d::Vertex3), &r);
   
    glBindBuffer(GL_ARRAY_BUFFER, 0);


    return r;
}