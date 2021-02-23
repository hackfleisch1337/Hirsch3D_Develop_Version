#include "../Include/Hirsch3D/core/Light.hpp"

void h3d::Light::init(glm::vec3 p, glm::vec3 c) {
    this->position = p;
    this->color = c;
}
void h3d::Light::setColor(glm::vec3 c) {
    this->color = c;
}
glm::vec3 h3d::Light::getColor() {
    return this->color;
}
void h3d::Light::setPosition(glm::vec3 p) {
    this->position = p;
}
glm::vec3 h3d::Light::getPosition() {
    return this->position;
}

void h3d::Light::translate(glm::vec3 p) {
    this->position += p;
}