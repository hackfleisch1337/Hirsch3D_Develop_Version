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

void h3d::Object::load(void* vertices, uint32_t amountOfVertices, uint32_t* indices, uint32_t amountOfIndices, glm::vec4 color, h3d::Texture* t) {
    this->amountOfVertices = amountOfVertices;
    this->vertices = new h3d::VertexBuffer(vertices, amountOfVertices);
    this->indices = new h3d::IndexBuffer(indices, amountOfIndices);
    this->hasLoaded = true;
    this->modelMatrix = glm::mat4(1.0f);
    this->color = color;
    this->position.x = 0;
    this->position.y = 0;
    this->position.z = 0;
    this->texture = t;
    this->rotationVector = glm::vec3(0.0f,0.0f,0.0f);
}

void h3d::Object::loadByPath(std::string path, glm::vec4 color) {

}

void h3d::Object::move(glm::vec3 d) {

    glm::vec3 r = rotationVector;
    glm::vec3 dnorm = glm::normalize(d);
    glm::vec3 dabs = glm::abs(d);
    
    glm::vec3 rnorm = glm::normalize(r);
    
    
    float angle = dnorm.x * rnorm.x + dnorm.y * rnorm.y + dnorm.z * rnorm.z;
    
    glm::vec4 d2 = glm::rotate(glm::mat4(1.0f), angle, glm::cross(rnorm, dnorm)) * glm::vec4(d, 1.0f);
    
    this->modelMatrix = glm::translate(this->modelMatrix, {d2.x, d2.y, d2.z});
    this->position += d;
}

void h3d::Object::moveInLineOfSight(glm::vec3 d) {
    this->modelMatrix = glm::translate(this->modelMatrix, d);
    this->position += d;
}

glm::vec3 h3d::Object::getPosition() {
    return this->position;
}

void h3d::Object::rotate(float degree, glm::vec3 direction) {
    this->modelMatrix = glm::rotate(this->modelMatrix, glm::radians(degree), direction);
    this->rotationVector += glm::normalize(direction) * degree;
}