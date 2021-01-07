#include "Object.hpp"


#ifndef GREEN
#define GREEN "\033[92m"
#define RESET_CLR "\x1B[0m"
#define RED "\033[91m"
#endif

h3d::Object::~Object() {
    if(hasLoaded) {
        delete this->vertices;
        delete this->indices;
        std::cout << GREEN  << "[OK] Deleted Object" << RESET_CLR << std::endl;
    }
}

void h3d::Object::load(void* vertices, uint32_t amountOfVertices, uint32_t* indices, uint32_t amountOfIndices, glm::vec4 color) {
    this->amountOfVertices = amountOfVertices;
    this->vertices = new h3d::VertexBuffer(vertices, amountOfVertices);
    this->indices = new h3d::IndexBuffer(indices, amountOfIndices);
    this->hasLoaded = true;
    this->modelMatrix = glm::mat4(1.0f);
    this->color = color;
}

void h3d::Object::loadByPath(std::string path, glm::vec4 color) {
}

void h3d::Object::rotate(float degree, glm::vec3 direction) {
    this->modelMatrix = glm::rotate(this->modelMatrix, glm::degrees(degree), direction);
}