#include "../Include/Hirsch3D/core/Scene.hpp"

void h3d::Scene::load(std::string vertexShaderSrc, std::string fragmentShaderSrc, std::string geometryShaderSrc,h3d::Camera* camera, float ambient) {
    this->shader.load(vertexShaderSrc, fragmentShaderSrc, geometryShaderSrc);
    this->camera = camera;
    this->ambient = ambient;
}

void h3d::Scene::load(h3d::Camera* camera, float ambient) {
    this->ambient = ambient;
    this->camera = camera;
    this->shader.load("./shader/main/shader.vert", "./shader/main/shader.frag", "./shader/main/shader.geo");
}

/**
 * @param o The pointer of the object to add
 *
 * Adds an object to the scene.
 *
 * @note Note that the object has to be
 * loaded before.
 *
 */
void h3d::Scene::addObject(h3d::Object* o) {
    if(!o->loaded()) {
        std::cout << RED << "[FAILED] Could not add object to scene (Object is not inititalized)" << RESET_CLR << std::endl;
        return;
    }
    this->objects.push_back(o);
}

void h3d::Scene::addLight(h3d::Light* l) {
    this->lights.push_back(l);
}

/**
 * @param r The h3d::Renderer r object
 *
 * Renders a 3D scene
 *
 */
void h3d::Scene::render(const h3d::Renderer &r) {

    this->shader.bind(); // bind scene shader

    // uniform float u_ambient
    int u_ambientUniformLocation = glGetUniformLocation(this->shader.getShaderId(), "u_ambient");
    glUniform1f(u_ambientUniformLocation, this->ambient);
/*
    int u_amountOfLights = glGetUniformLocation(this->shader.getShaderId(), "u_amountOfLights");
    int u_light_pos = glGetUniformLocation(this->shader.getShaderId(), "u_light_pos");
    int u_type = glGetUniformLocation(this->shader.getShaderId(), "u_light_type");
    int u_light_color = glGetUniformLocation(this->shader.getShaderId(), "u_light_color");
    int u_brightness = glGetUniformLocation(this->shader.getShaderId(), "u_brightness");
    
    glUniform1i(u_amountOfLights, this->lights.size());

    std::vector<glm::vec3> lightPositions;
    std::vector<glm::vec3> lightColors;
    std::vector<float> lightBrightness;
    std::vector<int> lightTypes;

    for(int i = 0; i < this->lights.size(); i++) {
        lightPositions.push_back(this->lights.at(i)->getPosition());
        lightColors.push_back(this->lights.at(i)->getColor());
        lightBrightness.push_back(this->lights.at(i)->getBrightness());
        lightTypes.push_back(this->lights.at(i)->getLightType());
    }
*/
    for(int i = 0; i < this->objects.size(); i++) {

        // uniform mat4 u_modelViewProj
        int u_modelUniformLocation = glGetUniformLocation(this->shader.getShaderId(), "u_modelViewProj");
        glm::mat4 m = this->camera->getViewProj() * this->objects.at(i)->getMatrix();

        // uniform vec4 u_color
        int u_colorUniformLocation = glGetUniformLocation(this->shader.getShaderId(), "u_color");
        glm::vec4 c = this->objects.at(i)->color;

        // uniform vec3 c_position
        int c_positionUniformLocation = glGetUniformLocation(this->shader.getShaderId(), "c_position");

        // uniform mat4 u_model
        int uniformU_Model = glGetUniformLocation(this->shader.getShaderId(), "u_model");
        glm::mat4 u_modelU = this->objects.at(i)->getMatrix();

        // uniform mat4 u_modelView
        int u_modelViewUniformLocation = glGetUniformLocation(this->shader.getShaderId(), "u_modelView");
        glm::mat4 mV = this->camera->getView() * this->objects.at(i)->getMatrix();

        // uniform mat4 u_invModelView
        int u_invModelViewUniformLocation = glGetUniformLocation(this->shader.getShaderId(), "u_invModelView");
        glm::mat4 invMv = glm::inverse(mV);

        // uniform mat4 u_view

        int u_viewUniformLocation = glGetUniformLocation(this->shader.getShaderId(), "u_view");
        glm::mat4 u_viewU = this->camera->getView();

        // Normalmap
        int u_normalmap = glGetUniformLocation(this->shader.getShaderId(), "u_normalmap");
        int isNormalSet = glGetUniformLocation(this->shader.getShaderId(), "isNormalSet");

        int u_roughnessmap = glGetUniformLocation(this->shader.getShaderId(), "u_roughnessmap");
        int isRoughnessSet = glGetUniformLocation(this->shader.getShaderId(), "isRoughnessSet");


        int u_kD = glGetUniformLocation(this->shader.getShaderId(), "u_kD");
        glUniform1f(u_kD, this->objects.at(i)->getMaterial().kD);

        int u_kS = glGetUniformLocation(this->shader.getShaderId(), "u_specIntensity");
        glUniform1f(u_kS, this->objects.at(i)->getMaterial().kS);

        int u_roughness = glGetUniformLocation(this->shader.getShaderId(), "u_shininess");
        glUniform1f(u_roughness, this->objects.at(i)->getMaterial().roughness);

        int u_specColor = glGetUniformLocation(this->shader.getShaderId(), "u_specColor");
        glm::vec3 u_specColorVector = this->objects.at(i)->getMaterial().specColor;
        glUniform3f(u_specColor, u_specColorVector.x, u_specColorVector.y, u_specColorVector.z);

        if(this->objects.at(i)->getNormalMap() != nullptr) {
            glUniform1i(isNormalSet, 1);
            this->objects.at(i)->getNormalMap()->bind();
            glUniform1i(u_normalmap, 1);
        } else {
            glUniform1i(isNormalSet, 0);
        }

        if(this->objects.at(i)->getRoughnessMap() != nullptr) {
            glUniform1i(isRoughnessSet, 1);
            this->objects.at(i)->getRoughnessMap()->bind();
            glUniform1i(u_roughnessmap, 2);
        } else {
            glUniform1i(isRoughnessSet, 0);
        }

        // Set uniforms
        glUniform4f(u_colorUniformLocation, c.r, c.g, c.b, c.a);
        glUniformMatrix4fv(u_modelUniformLocation, 1, GL_FALSE, &m[0][0]);
        glUniformMatrix4fv(u_modelViewUniformLocation, 1, GL_FALSE, &mV[0][0]);
        glUniform3f(c_positionUniformLocation, this->camera->getPosition().x,this->camera->getPosition().y, this->camera->getPosition().z);
        glUniformMatrix4fv(u_invModelViewUniformLocation, 1, GL_FALSE, &invMv[0][0]);
        glUniformMatrix4fv(uniformU_Model, 1, GL_FALSE, &u_modelU[0][0]);
        glUniformMatrix4fv(u_viewUniformLocation, 1, GL_FALSE, &u_viewU[0][0]);

        // Bind texture if available
        if(this->objects.at(i)->getTexture() != nullptr)
            this->objects.at(i)->getTexture()->bind();


        // Sets Texture Uniform
        if(this->objects.at(i)->getTexture() != nullptr) {
            //std::cout << "Texture" << std::endl;

            // set u_texture if available
            int textureUniformLocation = glGetUniformLocation(this->shader.getShaderId(), "u_texture");
            glUniform1i(textureUniformLocation, 0);

            // sets uniform isSamplerSet to 1 (= true) to clarify that a texture (u_texture) should be used
            int isSamplerSetUniformLocation = glGetUniformLocation(this->shader.getShaderId(), "isSamplerSet");
            glUniform1i(isSamplerSetUniformLocation, 1);

        } else {
            //std::cout << "Texture2" << std::endl;

            // sets uniform isSamplerSet to 0 (= false) to clarify that theres no texture and a color (u_color) should be used
            int isSamplerSetUniformLocation = glGetUniformLocation(this->shader.getShaderId(), "isSamplerSet");
            glUniform1i(isSamplerSetUniformLocation, 0);
        }


        // sets uniform u_position to objects position
        int u_positionUniformLocation = glGetUniformLocation(this->shader.getShaderId(), "u_position");
        glm::vec3 p = this->objects.at(i)->position;
        glUniform3f(u_positionUniformLocation, p.x, p.y, p.z);


        r.renderObject(this->objects.at(i)); // finally renders the object

        if(this->objects.at(i)->getTexture() != nullptr)
            this->objects.at(i)->getTexture()->unbind(); // unbind texture if used
        if(this->objects.at(i)->getNormalMap() != nullptr) {
            this->objects.at(i)->getNormalMap()->unbind();
        }
        if(this->objects.at(i)->getRoughnessMap() != nullptr) {
            this->objects.at(i)->getRoughnessMap()->unbind();
        }
    }
    
    this->shader.unbind(); // unbind scene shader
}

void h3d::Scene2D::load2D(float screenWidth, float screenHeight) {
    this->c2d.init(screenWidth, screenHeight);
    this->load("shader/2d.vert", "shader/2d.frag", "nogeometryshader", &c2d);
}

void h3d::Scene2D::render(const h3d::Renderer &r) {
    this->shader.bind(); // Bind scene shader
    for(int i = 0; i < this->objects.size(); i++) {
        // TODO setModelMatrixUniform

        int u_modelUniformLocation = glGetUniformLocation(this->shader.getShaderId(), "u_model");
        glm::mat4 m = this->c2d.getViewProj() * this->objects.at(i)->getMatrix();
        int u_colorUniformLocation = glGetUniformLocation(this->shader.getShaderId(), "u_color");
        glm::vec4 c = this->objects.at(i)->color;
        glUniform4f(u_colorUniformLocation, c.r, c.g, c.b, c.a);
        glUniformMatrix4fv(u_modelUniformLocation, 1, GL_FALSE, &m[0][0]);

        if(this->objects.at(i)->getTexture() != nullptr) {
            //std::cout << "Texture" << std::endl;
            int textureUniformLocation = glGetUniformLocation(this->shader.getShaderId(), "u_texture");
            this->objects.at(i)->getTexture()->bind();
            glUniform1i(textureUniformLocation, 0);

            int isSamplerSetUniformLocation = glGetUniformLocation(this->shader.getShaderId(), "isSamplerSet");
            glUniform1i(isSamplerSetUniformLocation, 1);
        } else {//std::cout << "Texture2" << std::endl;
            int isSamplerSetUniformLocation = glGetUniformLocation(this->shader.getShaderId(), "isSamplerSet");
            glUniform1i(isSamplerSetUniformLocation, 0);

        }
        r.renderObject(this->objects.at(i));
        if(this->objects.at(i)->getTexture() != nullptr)
        this->objects.at(i)->getTexture()->unbind();

    }
    this->shader.unbind(); // Unbind scene shader
}
