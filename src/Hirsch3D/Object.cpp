#include "Object.hpp"


#ifndef GREEN
#define GREEN "\033[92m"
#define RESET_CLR "\x1B[0m"
#endif

h3d::Object::~Object() {
    if(hasLoaded) {
        delete this->vertices;
        std::cout << GREEN  << "[OK] Deleted Object" << RESET_CLR << std::endl;
    }
}

void h3d::Object::load(void* vertices, uint32_t amountOfVertices, std::string vertexShader, std::string fragmentShader) {
    this->amountOfVertices = amountOfVertices;
    this->vertices = new h3d::VertexBuffer(vertices, amountOfVertices);
    this->hasLoaded = true;
    this->shader.load(vertexShader, fragmentShader);
}

void h3d::Object::loadByPath(std::string path) {
    // TODO
}