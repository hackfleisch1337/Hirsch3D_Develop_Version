#include "Scene.hpp"

void h3d::Scene::load(std::string vertexShaderSrc, std::string fragmentShaderSrc, h3d::Camera* camera) {
    this->shader.load(vertexShaderSrc, fragmentShaderSrc);
    this->camera = camera;
}

void h3d::Scene::addObject(h3d::Object* o) {
    if(!o->loaded()) {
        std::cout << RED << "[FAILED] Could not add object to scene (Object is not inititalized)" << RESET_CLR << std::endl;
        return;
    }
    this->objects.push_back(o);
}

void h3d::Scene::render(const h3d::Renderer &r) {
    this->shader.bind();
    for(int i = 0; i < this->objects.size(); i++) {
        // TODO setModelMatrixUniform
        int u_modelUniformLocation = glGetUniformLocation(this->shader.getShaderId(), "u_model");
        glm::mat4 m = this->camera->getViewProj() * this->objects.at(i)->getMatrix();
        int u_colorUniformLocation = glGetUniformLocation(this->shader.getShaderId(), "u_color");
        glm::vec4 c = this->objects.at(i)->color;
        glUniform4f(u_colorUniformLocation, c.r, c.g, c.b, c.a);
        glUniformMatrix4fv(u_modelUniformLocation, 1, GL_FALSE, &m[0][0]);
        r.renderObject(this->objects.at(i));
    }
    this->shader.unbind();
}
