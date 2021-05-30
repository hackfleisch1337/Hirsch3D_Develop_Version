#include "../Include/Hirsch3D/core/Scene.hpp"

void h3d::Scene::load(std::string vertexShaderSrc, std::string fragmentShaderSrc, std::string geometryShaderSrc,h3d::Camera* camera, glm::vec2 size, float ambient) {
    this->shader.load(vertexShaderSrc, fragmentShaderSrc, geometryShaderSrc);
    this->camera = camera;
    this->ambient = ambient;
    this->isLoaded = true;
    this->cubemap = nullptr;
    this->size = size;
    this->fb.load(size, 6);
    this->blurFb1.load(size, 1);
    this->blurFb2.load(size, 1);

    tonemappingShader.load("shader/tonemapping/shader.vert", "shader/tonemapping/shader.frag", "nogeometryshader");
    this->tmfb.load(size, 1);;

    h3d::Vertex3 cube_v[] = H3D_CUBE_VERTICES(1,1,1);
    uint32_t cube_i[] = H3D_CUBE_INDICES;
    this->cube.load(cube_v, 8, cube_i, 36, {0,1,0,1}, nullptr, nullptr, nullptr);
    this->cubemapshader.load("shader/cubemap/cube.vert", "shader/cubemap/cube.frag", "nogeometryshader");
    this->fbs.load("shader/framebuffer/framebuffer.vert", "shader/framebuffer/framebuffer.frag");
    this->blurShader.load("shader/blur/blur.vert", "shader/blur/blur.frag");
    for(int i = 0; i < 4; i++) {
        db[i].load({1280,720}, {2048,2048});
    }
    shadowmapshader.load("shader/depthmap/depthmap.vert", "shader/depthmap/depthmap.frag");
    loadUniformLocations();

    std::cout << GREEN << "[Ok] Loaded scene" << RESET_CLR << std::endl;
}

void h3d::Scene::load(h3d::Camera* camera, glm::vec2 size, float ambient) {
    this->load("./shader/main/shader.vert", "./shader/main/shader.frag", "./shader/main/shader.geo", camera, size, ambient);
}


void h3d::Scene::showCubeMap(bool b) {
    this->isCubeMapShown = b;
}

void h3d::Scene::loadUniformLocations() {

    #define H3D_UL(x) glGetUniformLocation(this->shader.getShaderId(), x)

    amountOfDlights = H3D_UL("amountOfDlights");
    amountOfPlights = H3D_UL("amountOfPlights");
    amountOfSlights = H3D_UL("amountOfSlights");

    u_transparency = H3D_UL("u_transparency");

    u_isCubeMapSet = H3D_UL("u_isCubeMapSet");
    u_cubemap  = H3D_UL("u_cubemap");

    u_modelViewProj = H3D_UL("u_modelViewProj");
    u_color = H3D_UL("u_color");
    c_position = H3D_UL("c_position");
    u_model = H3D_UL("u_model");
    u_modelView = H3D_UL("u_modelView");
    u_invModelView = H3D_UL("u_invModelView");
    u_view = H3D_UL("u_view");
    u_normalmap = H3D_UL("u_normalmap");
    isNormalSet = H3D_UL("isNormalSet");
    u_roughnessmap = H3D_UL("u_roughnessmap");
    isRoughnessSet = H3D_UL("isRoughnessSet");
    u_isMetallicSet = H3D_UL("u_isMetallicSet");
    u_metallicMap = H3D_UL("u_metallicMap");

    u_kD = H3D_UL("u_kD");
    u_specIntensity = H3D_UL("u_specIntensity");
    u_shininess = H3D_UL("u_shininess");
    u_specColor = H3D_UL("u_specColor");
    u_metallic = H3D_UL("u_metallic");
    u_emmisive = H3D_UL("u_emmisive");
    u_reflection = H3D_UL("u_reflection");
    u_refractionIndex = H3D_UL("u_refractionIndex");
    u_solidColor = H3D_UL("u_solidColor");
    u_texture = H3D_UL("u_texture");
    isSamplerSet = H3D_UL("isSamplerSet");
    u_position = H3D_UL("u_position");
    catchesShadow = H3D_UL("catchesShadow");
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


void h3d::Scene::setBloom(bool b, uint32_t amount, float brightness) {
    this->bloom = b;
    this->amountOfGaussianBlur = amount;
    this->bloomBrightness = brightness;
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

    glm::mat4 lightSpace[4];
    for(int i = 0; i < 4;i++) {
        lightSpace[i] = glm::mat4(1.0f);
    }

    for(int i = 0; i < dlights.size(); i++) {
        if(dlights.at(i)->shadowMap <= -1 || dlights.at(i)->shadowMap > 3) continue;
        int mapindex = dlights.at(i)->shadowMap;
        glViewport(0,0,db[mapindex].getShadowSize().x, db[mapindex].getShadowSize().y);
        db[mapindex].bind();
    
        glClear(GL_DEPTH_BUFFER_BIT);
        shadowmapshader.bind();
        float near_plane = 1.0f, far_plane = 100.5f;
        glm::mat4 lightProjection = glm::ortho(-30.0f, 30.0f, -30.0f, 30.0f, near_plane, far_plane);
        glm::mat4 lightView = glm::lookAt(glm::vec3(dlights.at(i)->direction), 
                                    glm::vec3( 0.0f, 0.0f,  0.0f), 
                                    glm::vec3( 0.0f, 1.0f,  0.0f));
        lightSpace[mapindex] = lightProjection * lightView;
        glUniformMatrix4fv(glGetUniformLocation(shadowmapshader.getShaderId(), "lightSpace"), 1, GL_FALSE, &lightSpace[mapindex][0][0]);
        //glCullFace(GL_FRONT);
        for(int j = 0; j < this->objects.size(); j++) {
            if(!objects.at(j)->castShadow) continue;
            glm::mat4 dbmodel = objects.at(j)->getMatrix();
            glUniformMatrix4fv(glGetUniformLocation(shadowmapshader.getShaderId(), "u_model"), 1, GL_FALSE, &dbmodel[0][0]);
            r.renderObject(objects.at(j));
        }
        //glCullFace(GL_BACK);
        shadowmapshader.unbind();
        db[mapindex].unbind();

    }

    // Render shadow map


    glViewport(0,0, this->size.x, this->size.y);
    this->fb.bind();
    glClearColor(0,0,0,0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    if(this->cubemap != nullptr && isCubeMapShown) {
        glDisable(GL_DEPTH_TEST);
        glDisable(GL_CULL_FACE);
       
        this->cubemapshader.bind();
        this->cubemap->bind();
        glUniform1i(glGetUniformLocation(this->cubemapshader.getShaderId(), "u_cubemap"), 3);
        glm::mat4 cubemapmat = this->camera->getView();
        glm::mat4 cubemapproj = this->camera->getProj();
        glUniformMatrix4fv(glGetUniformLocation(this->cubemapshader.getShaderId(), "u_view"), 1, GL_FALSE, &cubemapmat[0][0]);
        glUniformMatrix4fv(glGetUniformLocation(this->cubemapshader.getShaderId(), "u_proj"), 1, GL_FALSE, &cubemapproj[0][0]);

        r.renderObject(&this->cube); // RENDER
       
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

    glUniform1i(amountOfDlights, this->dlights.size());
    glUniform1i(amountOfPlights, this->plights.size());
    glUniform1i(amountOfSlights, this->slights.size());
    
    glUniform1i(u_transparency, this->transparency ? 1:0);
    
    glActiveTexture(GL_TEXTURE5);
    glBindTexture(GL_TEXTURE_2D, db[0].getTexture());
    glUniform1i(glGetUniformLocation(shaderId, "u_shadowMap0"), 5);
    glActiveTexture(GL_TEXTURE6);
    glBindTexture(GL_TEXTURE_2D, db[1].getTexture());
    glUniform1i(glGetUniformLocation(shaderId, "u_shadowMap1"), 6);
    glActiveTexture(GL_TEXTURE7);
    glBindTexture(GL_TEXTURE_2D, db[2].getTexture());
    glUniform1i(glGetUniformLocation(shaderId, "u_shadowMap2"), 7);
    glActiveTexture(GL_TEXTURE8);
    glBindTexture(GL_TEXTURE_2D, db[3].getTexture());
    glUniform1i(glGetUniformLocation(shaderId, "u_shadowMap3"), 8);

    glUniformMatrix4fv(glGetUniformLocation(shaderId, "u_lightspace0"), 1, GL_FALSE, &lightSpace[0][0][0]);
    glUniformMatrix4fv(glGetUniformLocation(shaderId, "u_lightspace1"), 1, GL_FALSE, &lightSpace[1][0][0]);
    glUniformMatrix4fv(glGetUniformLocation(shaderId, "u_lightspace2"), 1, GL_FALSE, &lightSpace[2][0][0]);
    glUniformMatrix4fv(glGetUniformLocation(shaderId, "u_lightspace3"), 1, GL_FALSE, &lightSpace[3][0][0]);
    

    

    if(this->cubemap != nullptr) {
        this->cubemap->bind();
        glUniform1i(u_isCubeMapSet, 1);
        glUniform1i(u_cubemap, 3);
    } else {
        glUniform1i(u_isCubeMapSet, 0);
        glUniform1i(u_cubemap, 3);
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
        uint32_t sm = this->dlights.at(i)->shadowMap;
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
        glUniform1i(glGetUniformLocation(shaderId, (uniformname + std::string(".shadowMap")).data()), 
            sm
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

        glUniform1i(catchesShadow, currentObject->catchShadow?1:0);

        if(currentObject->getTransparency() && this->transparency) {
            glDisable(GL_DEPTH_TEST);
        } else {
            glEnable(GL_DEPTH_TEST);
        }

        // uniform mat4 u_modelViewProj
        int u_modelUniformLocation = u_modelViewProj;
        glm::mat4 m = this->camera->getViewProj() * currentObject->getMatrix();

        // uniform vec4 u_color
        int u_colorUniformLocation = u_color;
        glm::vec4 c = currentObject->color;

        // uniform vec3 c_position
        int c_positionUniformLocation = c_position;

        // uniform mat4 u_model
        int uniformU_Model = u_model;
        glm::mat4 u_modelU = currentObject->getMatrix();

        // uniform mat4 u_modelView
        int u_modelViewUniformLocation = u_modelView;
        glm::mat4 mV = this->camera->getView() * currentObject->getMatrix();

        // uniform mat4 u_invModelView
        int u_invModelViewUniformLocation = u_invModelView;
        glm::mat4 invMv = glm::inverse(mV);

        // uniform mat4 u_view

        int u_viewUniformLocation = u_view;
        glm::mat4 u_viewU = this->camera->getView();

        // Normalmap

        glUniform1f(u_kD, currentObject->getMaterial().kD);

        
        glUniform1f(u_specIntensity, currentObject->getMaterial().kS);

        
        glUniform1f(u_shininess, currentObject->getMaterial().roughness);

        
        glm::vec3 u_specColorVector = currentObject->getMaterial().specColor;
        glUniform3f(u_specColor, u_specColorVector.x, u_specColorVector.y, u_specColorVector.z);

        
        glm::vec3 u_emmisive3f = currentObject->getMaterial().emmisive;
        glUniform3f(u_emmisive, u_emmisive3f.x, u_emmisive3f.y, u_emmisive3f.z);

        
        glUniform1f(u_reflection, currentObject->getMaterial().reflection);

        glUniform1f(u_refractionIndex, currentObject->getMaterial().refractionIndex);
        glUniform1f(u_solidColor, currentObject->getMaterial().solidColor);
        glUniform1f(u_metallic, currentObject->getMaterial().metallic);


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

        if(currentObject->getMetallicMap() != nullptr) {
            glUniform1i(u_isMetallicSet, 1);
            currentObject->getMetallicMap()->bind();
            glUniform1i(u_metallicMap, 4);
        } else {
            glUniform1i(u_isMetallicSet, 0);
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

        // Binds Reflection Cube Map if available
        if(currentObject->getReflectionCubeMap() != nullptr) {
            currentObject->getReflectionCubeMap()->bind();
        } else {
            this->cubemap->bind();
        }

        // Sets Texture Uniform
        if(currentObject->getTexture() != nullptr) {
            //std::cout << "Texture" << std::endl;

            // set u_texture if available
            glUniform1i(u_texture, 0);
            // sets uniform isSamplerSet to 1 (= true) to clarify that a texture (u_texture) should be used
            glUniform1i(isSamplerSet, 1);

        } else {
            // sets uniform isSamplerSet to 0 (= false) to clarify that theres no texture and a color (u_color) should be used
            glUniform1i(isSamplerSet, 0);
        }

        // sets uniform u_position to objects position
        glm::vec3 p = currentObject->getPosition();
        glUniform3f(u_position, p.x, p.y, p.z);


        r.renderObject(currentObject); // finally renders the object


        if(currentObject->getTexture() != nullptr)
            currentObject->getTexture()->unbind(); // unbind texture if used
        if(currentObject->getNormalMap() != nullptr) {
            currentObject->getNormalMap()->unbind();
        }
        if(currentObject->getRoughnessMap() != nullptr) {
            currentObject->getRoughnessMap()->unbind();
        }
        if(currentObject->getMetallicMap() != nullptr) {
            currentObject->getMetallicMap()->unbind();
        }
        if(currentObject->getReflectionCubeMap() != nullptr) {
            currentObject->getReflectionCubeMap()->unbind();
        }
        
    }
    if(this->cubemap != nullptr) {
        this->cubemap->unbind();
    }

    glActiveTexture(GL_TEXTURE5);
    glBindTexture(GL_TEXTURE_2D, 0);
    

    this->shader.unbind(); // unbind scene shader //
    this->fb.unbind();


    glActiveTexture(GL_TEXTURE0);


    // Gaussian blur //
    bool firstIteration = true;
    if(bloom) {
        for(int i= 0; i < amountOfGaussianBlur; i++) {
            this->blurFb1.bind();
            glClearColor(0,0,0,0);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            this->blurShader.bind();
            glUniform1i(glGetUniformLocation(blurShader.getShaderId(), "u_texture"), 0);
            glUniform1i(glGetUniformLocation(blurShader.getShaderId(), "horizontal"), 1);
            glBindTexture(GL_TEXTURE_2D, firstIteration ? fb.getRenderTarget(1):blurFb2.getRenderTarget(0));
            glDrawArrays(GL_TRIANGLES, 0, 3);
            this->blurShader.unbind();
            this->blurFb1.unbind();

            this->blurFb2.bind();
            glClearColor(0,0,0,0);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            this->blurShader.bind();
            glUniform1i(glGetUniformLocation(blurShader.getShaderId(), "u_texture"), 0);
            glUniform1i(glGetUniformLocation(blurShader.getShaderId(), "horizontal"), 0);
            glBindTexture(GL_TEXTURE_2D, blurFb1.getRenderTarget(0));
            glDrawArrays(GL_TRIANGLES, 0, 3);
            this->blurShader.unbind();
            this->blurFb2.unbind();

            firstIteration = false;
        }
    }
    
    // Tone Mapping

    glEnable(GL_BLEND);

    this->tmfb.bind();
    tonemappingShader.bind();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, fb.getRenderTarget(0));
    glUniform1i(glGetUniformLocation(tonemappingShader.getShaderId(), "u_texture"), 0);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, blurFb2.getRenderTarget(0));
    glUniform1i(glGetUniformLocation(tonemappingShader.getShaderId(), "u_brightTexture"), 1);
    glUniform1i(glGetUniformLocation(tonemappingShader.getShaderId(), "u_blur"), bloom);
    glUniform1f(glGetUniformLocation(tonemappingShader.getShaderId(), "u_blurBrightness"), bloomBrightness);
    glUniform1f(glGetUniformLocation(tonemappingShader.getShaderId(), "u_gamma"), this->gamma);
    glUniform1f(glGetUniformLocation(tonemappingShader.getShaderId(), "u_exposure"), this->exposure);

    glActiveTexture(GL_TEXTURE0);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    
    tonemappingShader.unbind();
    this->tmfb.unbind();



    // Render to the screen;
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glEnable(GL_BLEND);
    fbs.bind();
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, tmfb.getRenderTarget(0));
    glUniform1i(glGetUniformLocation(fbs.getShaderId(), "u_texture"), 0);
    if(this->ssr) {
        glUniform1i(glGetUniformLocation(fbs.getShaderId(), "ssrenabled"), 1);

        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, fb.getRenderTarget(2));
        glUniform1i(glGetUniformLocation(fbs.getShaderId(), "u_roughness"), 2);

        glActiveTexture(GL_TEXTURE3);
        glBindTexture(GL_TEXTURE_2D, fb.getRenderTarget(3));
        glUniform1i(glGetUniformLocation(fbs.getShaderId(), "u_normal"), 3);

        glActiveTexture(GL_TEXTURE4);
        glBindTexture(GL_TEXTURE_2D, fb.getRenderTarget(5));
        glUniform1i(glGetUniformLocation(fbs.getShaderId(), "u_depth"), 4);

        glActiveTexture(GL_TEXTURE5);
        glBindTexture(GL_TEXTURE_2D, fb.getRenderTarget(4));
        glUniform1i(glGetUniformLocation(fbs.getShaderId(), "u_metallic"), 5);

        glm::mat4 cview = this->camera->getView();
        glm::mat4 invView = glm::inverse(cview);
        glm::mat4 cproj = this->camera->getProj();
        glm::mat4 invProj = glm::inverse(cproj);
        glUniformMatrix4fv(glGetUniformLocation(fbs.getShaderId(), "view"), 1, GL_FALSE, &cview[0][0]);
        glUniformMatrix4fv(glGetUniformLocation(fbs.getShaderId(), "invView"), 1, GL_FALSE, &invView[0][0]);

        glUniformMatrix4fv(glGetUniformLocation(fbs.getShaderId(), "projection"), 1, GL_FALSE, &cproj[0][0]);
        glUniformMatrix4fv(glGetUniformLocation(fbs.getShaderId(), "invprojection"), 1, GL_FALSE, &invProj[0][0]);

    } else {
        glUniform1i(glGetUniformLocation(fbs.getShaderId(), "ssrenabled"), 0);
    }
    glActiveTexture(GL_TEXTURE0);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    fbs.unbind();

    glDisable(GL_BLEND);
}

void h3d::Scene::setCubeMap(h3d::CubeMap* c) {
    if(!c->loaded()) {
        throw h3d::Exception("Unable to set not loaded cubemap");
        return;
    }
    this->cubemap = c;
}

h3d::CubeMap* h3d::Scene::getCubeMap() {
    return this->cubemap;
}

void h3d::Scene::enableSSR(bool ssr) {
    this->ssr = ssr;
}

bool h3d::Scene::isSSRenabled() {
    return this->ssr;
}

void h3d::Scene2D::load2D(float screenWidth, float screenHeight) {
    this->c2d.init(screenWidth, screenHeight);
    this->load("shader/2d.vert", "shader/2d.frag", "nogeometryshader", &c2d, {screenWidth, screenHeight});
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


void h3d::Scene::setGamma(float gamma) {
    this->gamma = gamma;
}
void h3d::Scene::setExposure(float exp) {
    this->exposure = exp;
}

float h3d::Scene::getGamma() {
    return this->gamma;
}
float h3d::Scene::getExposure() {
    return this->exposure;
}

h3d::Scene::~Scene() {

}


void h3d::Scene::renderToCubeMap(h3d::CubeMap* cubemap) {
    // ...
    
    for(int i = 0; i < 6; i++) {

    }
}