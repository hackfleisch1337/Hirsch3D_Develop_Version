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
        if(this->objects.at(i)->getTexture() != nullptr)
            this->objects.at(i)->getTexture()->bind();
        if(this->objects.at(i)->getTexture() != nullptr) {
            //std::cout << "Texture" << std::endl;
            int textureUniformLocation = glGetUniformLocation(this->shader.getShaderId(), "u_texture");
            
            glUniform1i(textureUniformLocation, 0);
            
            
            int isSamplerSetUniformLocation = glGetUniformLocation(this->shader.getShaderId(), "isSamplerSet");
            glUniform1i(isSamplerSetUniformLocation, 1);
        } else {
            //std::cout << "Texture2" << std::endl;
            int isSamplerSetUniformLocation = glGetUniformLocation(this->shader.getShaderId(), "isSamplerSet");
            glUniform1i(isSamplerSetUniformLocation, 0);
            
        }
        
        r.renderObject(this->objects.at(i));
        if(this->objects.at(i)->getTexture() != nullptr)
            this->objects.at(i)->getTexture()->unbind();
    }
    this->shader.unbind();
}

void h3d::Scene2D::load2D() {
    this->c2d.init();
    c2d.translate({0,0,5});
    this->load("src/Hirsch3D/shader/2d.vert", "src/Hirsch3D/shader/2d.frag", &c2d);
}

void h3d::Scene2D::render(const h3d::Renderer &r) {
    this->shader.bind();
    for(int i = 0; i < this->objects.size(); i++) {
        // TODO setModelMatrixUniform
        
        int u_modelUniformLocation = glGetUniformLocation(this->shader.getShaderId(), "u_model");
        glm::mat4 m = this->objects.at(i)->getMatrix();
        int u_colorUniformLocation = glGetUniformLocation(this->shader.getShaderId(), "u_color");
        glm::vec4 c = this->objects.at(i)->color;
        glUniform4f(u_colorUniformLocation, c.r, c.g, c.b, c.a);
        glUniformMatrix4fv(u_modelUniformLocation, 1, GL_FALSE, &m[0][0]);

        if(this->objects.at(i)->getTexture() != nullptr) {
            //std::cout << "Texture" << std::endl;
            int textureUniformLocation = glGetUniformLocation(this->shader.getShaderId(), "u_texture");
            this->objects.at(i)->getTexture()->bind();
            glUniform1i(textureUniformLocation, 0);
            this->objects.at(i)->getTexture()->unbind();
            
            int isSamplerSetUniformLocation = glGetUniformLocation(this->shader.getShaderId(), "isSamplerSet");
            glUniform1i(isSamplerSetUniformLocation, 1);
        } else {//std::cout << "Texture2" << std::endl;
            int isSamplerSetUniformLocation = glGetUniformLocation(this->shader.getShaderId(), "isSamplerSet");
            glUniform1i(isSamplerSetUniformLocation, 0);
            
        }
        

        r.renderObject(this->objects.at(i));
    }
    this->shader.unbind();
}