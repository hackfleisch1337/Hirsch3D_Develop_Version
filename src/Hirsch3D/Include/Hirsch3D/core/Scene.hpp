/**
 * Hirsch3D Renderengine v.0.10.1
 * For more informations see README.md
 * or read the documentation in the doc folder
 * GitHub Repository: https://github.com/hackfleisch1337/Hirsch3D_Release_v1.0
 * Licence: MIT Licence
 * Last changes: 02.03.2021 (DD.MM.YYYY)
 * 
 * @file Hirsch3D/core/Scene.hpp
 * @ref Hirsch3d/core
 *
 * @author Emanuel Zache
 * @version 0.9.7
 * @copyright (C) 2021 Emanuel Zache
 *
 *
 * 
 * 
 * 
 * 
 * 
 * 
 *
 */


#ifndef HIRSCH3D_SCENE_HPP
#define HIRSCH3D_SCENE_HPP
#include <vector>
#include "Object.hpp"
#include "../Hirsch3D.hpp"
#include "Camera.hpp"
#include "DepthBuffer.hpp"


namespace h3d
{
    class Renderer;

    /**
     * Renderstate for 3D objects
     *
     *
     *
     *
     *
     *
     */
    class Scene {
    public:
        /**
         * Inititalizes the scene
         * 
         * @param vertexShaderSrc path to the vertex shader source file
         * @param fragmentShaderSrc path to the fragment shader source file
         * @param geometryShaderSrc path to the geometry shader source file
         * @param camera Pointer to the scenes camera
         * @param ambient intensity of the scenes ambient light (default=0.1f)
         * 
         */
        void load(std::string vertexShaderSrc, std::string fragmentShaderSrc, std::string geometryShaderSrc,h3d::Camera* camera, glm::vec2 size,float ambient = 0.1f);
        
        /**
         * Inititalizes the scene with the default shader
         * 
         * @param camera Pointer to the scenes camera
         * @param ambient intensity of the scenes ambient light (default=0.1f)
         * 
         */
        void load(h3d::Camera* camera, glm::vec2 size, float ambient = 0.1f);

        /**
         * 
         * Destructor
         */
        virtual ~Scene();

        /**
         * Adds an object to the scene
         * 
         * @param o A pointer to the object
         * 
         */
        void addObject(h3d::Object* o);

        /**
         * Adds a directional light to the scene
         * 
         * @param l A pointer to the directional light
         * 
         */
        void addDirectionalLight(h3d::DirectionalLight* l);

        /**
         * Adds a point light to the scene
         * 
         * @param l A pointer to the point light
         * 
         */
        void addPointLight(h3d::PointLight* l);

        /**
         * Adds a spot light to the scene
         * 
         * @param l A pointer to the spot light
         * 
         */
        void addSpotLight(h3d::SpotLight* l);

        /**
         * Renders the scene
         * 
         * 
         * @param r The Hirsch3D Renderer
         */
        virtual void render(const h3d::Renderer &r);
        
        /**
         * Enables/Disables the transparency
         * No transparency means better performance
         * 
         * @param t True if the scene should have transparency
         */
        void setTransparency(bool t);


        /**
         * @returns true if transparency is enabled
         */
        bool hasTransparency();

        /**
         * Sets a cube envoirement map for the scene
         * @param c The cubemap
         */
        void setCubeMap(h3d::CubeMap* c);

        /**
         * @returns The cubemap
         */
        h3d::CubeMap* getCubeMap();


        /**
         * Enables/Disables a bloom effect for the bright parts on the screen.
         * WARNING: This will have an extreme effect on the performance and could cause lags.
         * 
         * @param b Enable/Disable
         * @param amount The number of executions of the Gaussian Blur. The higher this value, the stronger
         *               the blur effect but it has a higher effect on the performance
         * 
         */
        void setBloom(bool b, uint32_t amount, float brightness=2.5);

        void renderToCubeMap(h3d::CubeMap* cubemap);

        void showCubeMap(bool b);

        void setGamma(float g);
        void setExposure(float e);

        float getGamma();
        float getExposure();
        
        void enableSSR(bool ssr);

        bool isSSRenabled();


        float ambient;
    protected:
        h3d::Shader shader;
        std::vector<h3d::Object*> objects;
        std::vector<h3d::DirectionalLight*> dlights;
        std::vector<h3d::PointLight*> plights;
        std::vector<h3d::SpotLight*> slights;
        h3d::Camera* camera;
        bool isLoaded = false;
        glm::vec2 size;
        h3d::FrameBuffer fb;
        h3d::Shader fbs;
        
        float gamma = 1.0f;
        float exposure = 1.0f;

    private:    
        h3d::DepthBuffer db[4];
        h3d::Shader shadowmapshader;
        
        h3d::Shader blurShader;
        h3d::FrameBuffer blurFb1;
        h3d::FrameBuffer blurFb2;
    
        bool transparency = false;
        h3d::CubeMap* cubemap = nullptr;
        h3d::Shader cubemapshader;
        h3d::Object cube;

        bool bloom = false;
        uint32_t amountOfGaussianBlur = 10;
        float bloomBrightness = 2.5;

        bool isCubeMapShown = true;
        
        bool ssr = false;

        void loadUniformLocations();

        // Uniform locations;
        uint32_t amountOfDlights;
        uint32_t amountOfPlights;
        uint32_t amountOfSlights;

        uint32_t u_transparency;

        uint32_t u_isCubeMapSet;
        uint32_t u_cubemap;

        uint32_t u_modelViewProj;
        uint32_t u_color;
        uint32_t c_position;
        uint32_t u_model;
        uint32_t u_modelView;
        uint32_t u_invModelView;
        uint32_t u_view;
        uint32_t u_normalmap;
        uint32_t isNormalSet;
        uint32_t u_roughnessmap;
        uint32_t isRoughnessSet;
        uint32_t u_isMetallicSet;
        uint32_t u_metallicMap;

        uint32_t u_kD;
        uint32_t u_specIntensity;
        uint32_t u_metallic;
        uint32_t u_shininess;
        uint32_t u_specColor;
        uint32_t u_emmisive;
        uint32_t u_reflection;
        uint32_t u_refractionIndex;
        uint32_t u_solidColor;
        uint32_t u_texture;
        uint32_t isSamplerSet;
        uint32_t u_position;
        uint32_t catchesShadow;


    };

    /**
     * Renderstate to render 2D objects
     *
     *
     *
     */
    class Scene2D: public Scene {
    public:
        /**
         * Loads a 2D scene
         * 
         * @param width The width of the scene in pixels
         * @param height The height of the scene in pixels
         * 
         */
        void load2D(float width, float height);

        /**
         * Renders the scene
         * 
         * 
         * @param r The Hirsch3D Renderer
         */
        void render(const h3d::Renderer &r) override;
    private:

        h3d::Camera c2d;
    };

} // namespace h3d


#endif
