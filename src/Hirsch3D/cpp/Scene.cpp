#include "../Include/Hirsch3D/core/Scene.hpp"

void h3d::Scene::load(std::string vertexShaderSrc, std::string fragmentShaderSrc, std::string geometryShaderSrc,h3d::Camera* camera, float ambient) {
    this->shader.load(vertexShaderSrc, fragmentShaderSrc, geometryShaderSrc);
    this->camera = camera;
    this->ambient = ambient;
    this->isLoaded = true;
    this->cubemap = nullptr;
    h3d::Vertex3 cube_v[] = H3D_CUBE_VERTICES(1,1,1);
    uint32_t cube_i[] = H3D_CUBE_INDICES;
    this->cube.load(cube_v, 8, cube_i, 36, {0,1,0,1}, nullptr, nullptr, nullptr);
    this->cubemapshader.load("shader/cubemap/cube.vert", "shader/cubemap/cube.frag", "nogeometryshader");
    std::cout << GREEN << "[Ok] Loaded scene" << RESET_CLR << std::endl;
}

void h3d::Scene::load(h3d::Camera* camera, float ambient) {
    this->cubemap = nullptr;
    this->ambient = ambient;
    this->camera = camera;
    this->shader.load("./shader/main/shader.vert", "./shader/main/shader.frag", "./shader/main/shader.geo");
    this->isLoaded = true;
    h3d::Vertex3 cube_v[] = H3D_CUBE_VERTICES(1,1,1);
    uint32_t cube_i[] = H3D_CUBE_INDICES;
    this->cube.load(cube_v, 8, cube_i, 36, {0,1,0,1}, nullptr, nullptr, nullptr);
    this->cubemapshader.load("shader/cubemap/cube.vert", "shader/cubemap/cube.frag", "nogeometryshader");
    std::cout << GREEN << "[Ok] Loaded scene" << RESET_CLR << std::endl;
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
        throw h3d::Exception("Could not add uninitialized objects to scene");
        return;
    }
    this->objects.push_back(o);
}

void h3d::Scene::addDirectionalLight(h3d::DirectionalLight* l) {
    if(this->dlights.size() < MAX_LIGHTS_) {
        this->dlights.push_back(l);
    } else {
        throw h3d::Exception("Maximum amount of lights already reached");
    }
}
void h3d::Scene::addPointLight(h3d::PointLight* l) {
    if(this->plights.size() < MAX_LIGHTS_) {
        this->plights.push_back(l);
    } else {
        throw h3d::Exception("Maximum amount of lights already reached");
    }
}
void h3d::Scene::addSpotLight(h3d::SpotLight* l) {
    if(this->slights.size() < MAX_LIGHTS_) {
        this->slights.push_back(l);
    } else {
        throw h3d::Exception("Maximum amount of lights already reached");
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
    if(!this->isLoaded) {
        throw h3d::Exception("Can not render uninitialized Scene");
        return;
    }

    if(this->cubemap != nullptr) {
        glDisable(GL_DEPTH_TEST);
        glDisable(GL_CULL_FACE);
        this->cubemapshader.bind();
        this->cubemap->bind();
        glUniform1i(glGetUniformLocation(this->cubemapshader.getShaderId(), "u_cubemap"), 3);
        glm::mat4 cubemapmat = this->camera->getView();
        glm::mat4 cubemapproj = this->camera->getProj();
        glUniformMatrix4fv(glGetUniformLocation(this->cubemapshader.getShaderId(), "u_view"), 1, GL_FALSE, &cubemapmat[0][0]);
        glUniformMatrix4fv(glGetUniformLocation(this->cubemapshader.getShaderId(), "u_proj"), 1, GL_FALSE, &cubemapproj[0][0]);
        r.renderObject(&this->cube);
        this->cubemap->unbind();
        this->cubemapshader.unbind();
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_CULL_FACE);
    }

    this->shader.bind(); // bind scene shader

    GLuint shaderId = this->shader.getShaderId();

    // uniform float u_ambient
    int u_ambientUniformLocation = glGetUniformLocation(shaderId, "u_ambient");
    glUniform1f(u_ambientUniformLocation, this->ambient);

    glUniform1i(glGetUniformLocation(shader.getShaderId(), "amountOfDlights"), this->dlights.size());
    glUniform1i(glGetUniformLocation(shader.getShaderId(), "amountOfPlights"), this->plights.size());
    glUniform1i(glGetUniformLocation(shader.getShaderId(), "amountOfSlights"), this->slights.size());
    
    glUniform1i(glGetUniformLocation(shaderId, "u_transparency"), this->transparency ? 1:0);

    
    
    if(this->cubemap != nullptr) {
        this->cubemap->bind();
        glUniform1i(glGetUniformLocation(shaderId, "u_isCubeMapSet"), 1);
        glUniform1i(glGetUniformLocation(shaderId, "u_cubemap"), 3);
    } else {
        glUniform1i(glGetUniformLocation(shaderId, "u_isCubeMapSet"), 0);
        glUniform1i(glGetUniformLocation(shaderId, "u_cubemap"), 3);
    }

    if(transparency) {
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    } else {
       glDisable(GL_BLEND);
    }
    
    for(int i = 0; i < dlights.size(); i++) {
        std::string uniformname = "dlights[" + std::to_string(i) + "]";
        glm::vec3 dir = this->dlights.at(i)->direction;
        glm::vec3 col = this->dlights.at(i)->color;
        float br = this->dlights.at(i)->brightness;
        //std::cout << dir.x << " " << dir.y << " " << dir.z << std::endl;
        glUniform3f(glGetUniformLocation(shaderId, (uniformname + std::string(".direction")).data()), 
            dir.x, dir.y, dir.z
        );
        
        glUniform3f(glGetUniformLocation(shaderId, (uniformname + std::string(".color")).data()), 
            col.x, col.y, col.z
        );
        glUniform1f(glGetUniformLocation(shaderId, (uniformname + std::string(".brightness")).data()), 
            br
        );
    }

    for(int i = 0; i < plights.size(); i++) {
        std::string uniformname = "plights[" + std::to_string(i) + "]";
        glUniform3f(glGetUniformLocation(shaderId, (uniformname + std::string(".position")).data()), 
            plights.at(i)->position.x, plights.at(i)->position.y, plights.at(i)->position.z
        );
        glUniform3f(glGetUniformLocation(shaderId, (uniformname + std::string(".color")).data()), 
            plights.at(i)->color.x, plights.at(i)->color.y, plights.at(i)->color.z
        );
        glUniform1f(glGetUniformLocation(shaderId, (uniformname + std::string(".brightness")).data()), 
            plights.at(i)->brightness
        );
        glUniform1f(glGetUniformLocation(shaderId, (uniformname + std::string(".linear")).data()), 
            plights.at(i)->linear
        );
        glUniform1f(glGetUniformLocation(shaderId, (uniformname + std::string(".quadratic")).data()), 
            plights.at(i)->quadratic
        );
    }

    for(int i = 0; i < slights.size(); i++) {
        std::string uniformname = "slights[" + std::to_string(i) + "]";
        glUniform3f(glGetUniformLocation(shaderId, (uniformname + std::string(".position")).data()), 
            slights.at(i)->position.x, slights.at(i)->position.y, slights.at(i)->position.z
        );
        glUniform3f(glGetUniformLocation(shaderId, (uniformname + std::string(".direction")).data()), 
            slights.at(i)->direction.x, slights.at(i)->direction.y, slights.at(i)->direction.z
        );
        glUniform3f(glGetUniformLocation(shaderId, (uniformname + std::string(".color")).data()), 
            slights.at(i)->color.x, slights.at(i)->color.y, slights.at(i)->color.z
        );
        glUniform1f(glGetUniformLocation(shaderId, (uniformname + std::string(".brightness")).data()), 
            slights.at(i)->brightness
        );
        glUniform1f(glGetUniformLocation(shaderId, (uniformname + std::string(".outerCone")).data()), 
            slights.at(i)->outerCone
        );
        glUniform1f(glGetUniformLocation(shaderId, (uniformname + std::string(".innerCone")).data()), 
            slights.at(i)->innerCone
        );
    }

    for(int i = 0; i < this->objects.size(); i++) {
        
        h3d::Object* currentObject = this->objects.at(i);

        if(!currentObject->isVisible()) {
            continue;
        }

        if(currentObject->getTransparency() && this->transparency) {
            glDisable(GL_DEPTH_TEST);
        } else {
            glEnable(GL_DEPTH_TEST);
        }

        // uniform mat4 u_modelViewProj
        int u_modelUniformLocation = glGetUniformLocation(shaderId, "u_modelViewProj");
        glm::mat4 m = this->camera->getViewProj() * currentObject->getMatrix();

        // uniform vec4 u_color
        int u_colorUniformLocation = glGetUniformLocation(shaderId, "u_color");
        glm::vec4 c = currentObject->color;

        // uniform vec3 c_position
        int c_positionUniformLocation = glGetUniformLocation(shaderId, "c_position");

        // uniform mat4 u_model
        int uniformU_Model = glGetUniformLocation(shaderId, "u_model");
        glm::mat4 u_modelU = currentObject->getMatrix();

        // uniform mat4 u_modelView
        int u_modelViewUniformLocation = glGetUniformLocation(shaderId, "u_modelView");
        glm::mat4 mV = this->camera->getView() * currentObject->getMatrix();

        // uniform mat4 u_invModelView
        int u_invModelViewUniformLocation = glGetUniformLocation(shaderId, "u_invModelView");
        glm::mat4 invMv = glm::inverse(mV);

        // uniform mat4 u_view

        int u_viewUniformLocation = glGetUniformLocation(shaderId, "u_view");
        glm::mat4 u_viewU = this->camera->getView();

        // Normalmap
        int u_normalmap = glGetUniformLocation(shaderId, "u_normalmap");
        int isNormalSet = glGetUniformLocation(shaderId, "isNormalSet");

        int u_roughnessmap = glGetUniformLocation(shaderId, "u_roughnessmap");
        int isRoughnessSet = glGetUniformLocation(shaderId, "isRoughnessSet");


        int u_kD = glGetUniformLocation(shaderId, "u_kD");
        glUniform1f(u_kD, currentObject->getMaterial().kD);

        int u_kS = glGetUniformLocation(shaderId, "u_specIntensity");
        glUniform1f(u_kS, currentObject->getMaterial().kS);

        int u_roughness = glGetUniformLocation(shaderId, "u_shininess");
        glUniform1f(u_roughness, currentObject->getMaterial().roughness);

        int u_specColor = glGetUniformLocation(shaderId, "u_specColor");
        glm::vec3 u_specColorVector = currentObject->getMaterial().specColor;
        glUniform3f(u_specColor, u_specColorVector.x, u_specColorVector.y, u_specColorVector.z);

        int u_emmisive = glGetUniformLocation(shaderId, "u_emmisive");
        glm::vec3 u_emmisive3f = currentObject->getMaterial().emmisive;
        glUniform3f(u_emmisive, u_emmisive3f.x, u_emmisive3f.y, u_emmisive3f.z);

        int u_reflection = glGetUniformLocation(shaderId, "u_reflection");
        glUniform1f(u_reflection, currentObject->getMaterial().reflection);

        if(currentObject->getNormalMap() != nullptr) {
            glUniform1i(isNormalSet, 1);
            currentObject->getNormalMap()->bind();
            glUniform1i(u_normalmap, 1);
        } else {
            glUniform1i(isNormalSet, 0);
        }

        if(currentObject->getRoughnessMap() != nullptr) {
            glUniform1i(isRoughnessSet, 1);
            currentObject->getRoughnessMap()->bind();
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
        if(currentObject->getTexture() != nullptr)
            currentObject->getTexture()->bind();


        // Sets Texture Uniform
        if(currentObject->getTexture() != nullptr) {
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
        glm::vec3 p = currentObject->getPosition();
        glUniform3f(u_positionUniformLocation, p.x, p.y, p.z);


        r.renderObject(currentObject); // finally renders the object

        if(currentObject->getTexture() != nullptr)
            currentObject->getTexture()->unbind(); // unbind texture if used
        if(currentObject->getNormalMap() != nullptr) {
            currentObject->getNormalMap()->unbind();
        }
        if(currentObject->getRoughnessMap() != nullptr) {
            currentObject->getRoughnessMap()->unbind();
        }
        if(this->cubemap != nullptr) {
            this->cubemap->unbind();
        }
    }
    
    this->shader.unbind(); // unbind scene shader
    glDisable(GL_BLEND);
}

void h3d::Scene::setCubeMap(h3d::CubeMap* c) {
    this->cubemap = c;
}

h3d::CubeMap* h3d::Scene::getCubeMap() {
    return this->cubemap;
}

void h3d::Scene2D::load2D(float screenWidth, float screenHeight) {
    this->c2d.init(screenWidth, screenHeight);
    this->load("shader/2d.vert", "shader/2d.frag", "nogeometryshader", &c2d);
}

void h3d::Scene2D::render(const h3d::Renderer &r) {
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    this->shader.bind(); // Bind scene shader
    for(int i = 0; i < this->objects.size(); i++) {

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
    glDisable(GL_BLEND);
}

void h3d::Scene::setTransparency(bool t) {
    this->transparency = t;
}

bool h3d::Scene::hasTransparency() {
    return this->transparency;
}