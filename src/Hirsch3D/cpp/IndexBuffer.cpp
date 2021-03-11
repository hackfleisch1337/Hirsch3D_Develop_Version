#include "../Include/Hirsch3D/core/IndexBuffer.hpp"
#ifndef GREEN
#define GREEN "\033[92m"
#define YELLOW "\033[93m"
#define RESET_CLR "\x1B[0m"
#endif

h3d::IndexBuffer::IndexBuffer(uint32_t* data, uint32_t amountOfIndices) {

    this->amountOfIndices = amountOfIndices;
    
    

    glGenBuffers(1, &this->bufferId);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->bufferId);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->amountOfIndices * sizeof(data[0]), data, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    std::cout << GREEN << "[Ok] Generated index buffer" << YELLOW << "(" << amountOfIndices << " indices)" << RESET_CLR << std::endl;
}

h3d::IndexBuffer::~IndexBuffer() {
    glDeleteBuffers(1, &this->bufferId);
    std::cout << GREEN << "[Ok] Deleted index buffer data" << RESET_CLR << std::endl;
}

void h3d::IndexBuffer::bind() {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->bufferId);
}

void h3d::IndexBuffer::unbind() {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

uint32_t h3d::IndexBuffer::getAmountOfIndices() {
    return this->amountOfIndices;
}