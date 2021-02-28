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

void h3d::Scene::addDirectionalLight(h3d::DirectionalLight* l) {
    if(this->dlights.size() < MAX_LIGHTS_) {
        this->dlights.push_back(l);
    }
}
void h3d::Scene::addPointLight(h3d::PointLight* l) {
    if(this->plights.size() < MAX_LIGHTS_) {
        this->plights.push_back(l);
    }
}
void h3d::Scene::addSpotLight(h3d::SpotLight* l) {
    if(this->slights.size() < MAX_LIGHTS_) {
        this->slights.push_back(l);
    }
}
/**
 * @param r The h3d::Renderer r object
 *
 * Renders a 3D scene
 *
 */
void h3d::Scene::render(const h3d::Renderer &r) {
    //std::cout << "RENDER" << std::endl;
    this->shader.bind(); // bind scene shader

    // uniform float u_ambient
    int u_ambientUniformLocation = glGetUniformLocation(this->shader.getShaderId(), "u_ambient");
    glUniform1f(u_ambientUniformLocation, this->ambient);

    glUniform1i(glGetUniformLocation(shader.getShaderId(), "amountOfDlights"), this->dlights.size());
    glUniform1i(glGetUniformLocation(shader.getShaderId(), "amountOfPlights"), this->plights.size());
    glUniform1i(glGetUniformLocation(shader.getShaderId(), "amountOfSlights"), this->slights.size());
    /*
    glUniform3f(glGetUniformLocation(shader.getShaderId(), "dlights[0].direction"), 
            0,0,1
    );
    
    glUniform3f(glGetUniformLocation(shader.getShaderId(), (std::string("dlights[") + std::to_string(0) + "].color").data()), 
           0.4f,1.0f,0.4f
    );
    glUniform1f(glGetUniformLocation(shader.getShaderId(), "dlights[0].brightness"), 
        1.0f
    );*/


    
    for(int i = 0; i < dlights.size(); i++) {
        std::string uniformname = "dlights[" + std::to_string(i) + "]";
        glm::vec3 dir = this->dlights.at(i)->direction;
        glm::vec3 col = this->dlights.at(i)->color;
        float br = this->dlights.at(i)->brightness;
        //std::cout << dir.x << " " << dir.y << " " << dir.z << std::endl;
        glUniform3f(glGetUniformLocation(shader.getShaderId(), (uniformname + std::string(".direction")).data()), 
            dir.x, dir.y, dir.z
        );
        
        glUniform3f(glGetUniformLocation(shader.getShaderId(), (uniformname + std::string(".color")).data()), 
            col.x, col.y, col.z
        );
        glUniform1f(glGetUniformLocation(shader.getShaderId(), (uniformname + std::string(".brightness")).data()), 
            br
        );
    }

    for(int i = 0; i < plights.size(); i++) {
        std::string uniformname = "plights[" + std::to_string(i) + "]";
        glUniform3f(glGetUniformLocation(shader.getShaderId(), (uniformname + std::string(".position")).data()), 
            plights.at(i)->position.x, plights.at(i)->position.y, plights.at(i)->position.z
        );
        glUniform3f(glGetUniformLocation(shader.getShaderId(), (uniformname + std::string(".color")).data()), 
            plights.at(i)->color.x, plights.at(i)->color.y, plights.at(i)->color.z
        );
        glUniform1f(glGetUniformLocation(shader.getShaderId(), (uniformname + std::string(".brightness")).data()), 
            plights.at(i)->brightness
        );
        glUniform1f(glGetUniformLocation(shader.getShaderId(), (uniformname + std::string(".linear")).data()), 
            plights.at(i)->linear
        );
        glUniform1f(glGetUniformLocation(shader.getShaderId(), (uniformname + std::string(".quadratic")).data()), 
            plights.at(i)->quadratic
        );
    }

    for(int i = 0; i < slights.size(); i++) {
        std::string uniformname = "slights[" + std::to_string(i) + "]";
        glUniform3f(glGetUniformLocation(shader.getShaderId(), (uniformname + std::string(".position")).data()), 
            slights.at(i)->position.x, slights.at(i)->position.y, slights.at(i)->position.z
        );
        glUniform3f(glGetUniformLocation(shader.getShaderId(), (uniformname + std::string(".direction")).data()), 
            slights.at(i)->direction.x, slights.at(i)->direction.y, slights.at(i)->direction.z
        );
        glUniform3f(glGetUniformLocation(shader.getShaderId(), (uniformname + std::string(".color")).data()), 
            slights.at(i)->color.x, slights.at(i)->color.y, slights.at(i)->color.z
        );
        glUniform1f(glGetUniformLocation(shader.getShaderId(), (uniformname + std::string(".brightness")).data()), 
            slights.at(i)->brightness
        );
        glUniform1f(glGetUniformLocation(shader.getShaderId(), (uniformname + std::string(".outerCone")).data()), 
            slights.at(i)->outerCone
        );
        glUniform1f(glGetUniformLocation(shader.getShaderId(), (uniformname + std::string(".innerCone")).data()), 
            slights.at(i)->innerCone
        );
    }

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
