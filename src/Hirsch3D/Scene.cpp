#include "Scene.hpp"

void h3d::Scene::load(std::string vertexShaderSrc, std::string fragmentShaderSrc) {
    this->shader.load(vertexShaderSrc, fragmentShaderSrc);
}

void h3d::Scene::addObject(h3d::Object* o) {
    this->objects.push_back(o);
}

void h3d::Scene::render(const h3d::Renderer &r) {
    this->shader.bind();
    for(int i = 0; i < this->objects.size(); i++) {
        // TODO setModelMatrixUniform
        int u_modelUniformLocation = glGetUniformLocation(this->shader.getShaderId(), "u_model");
        glm::mat4 m = this->objects.at(i)->getMatrix();
        
        glUniformMatrix4fv(u_modelUniformLocation, 1, GL_FALSE, &m[0][0]);
        r.renderObject(this->objects.at(i));
    }
    this->shader.unbind();
}
