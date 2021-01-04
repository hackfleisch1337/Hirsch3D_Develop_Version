#include "Object.hpp"


#ifndef GREEN
#define GREEN "\033[92m"
#define RESET_CLR "\x1B[0m"
#endif

h3d::Object::~Object() {
    delete this->vertices;
    std::cout << GREEN  << "[OK] Deleted Object" << RESET_CLR << std::endl;
}

void h3d::Object::load(void* vertices, uint32_t amountOfVertices) {
    this->amountOfVertices = amountOfVertices;
    this->vertices = new h3d::VertexBuffer(vertices, amountOfVertices);
}

void h3d::Object::loadByPath() {

}