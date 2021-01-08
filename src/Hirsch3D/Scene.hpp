#ifndef HIRSCH3D_SCENE_HPP
#define HIRSCH3D_SCENE_HPP
#include <vector>
#include "Object.hpp"
#include "Hirsch3D.hpp"
#include "Camera.hpp"


namespace h3d
{
    class Renderer;
    class Scene {
    public:
        void load(std::string vertexShaderSrc, std::string fragmentShaderSrc, h3d::Camera* camera);

        void addObject(h3d::Object* o);
        // void addLight(h3d::PointLight &p);
        virtual void render(const h3d::Renderer &r);
        

    protected:
        h3d::Shader shader;
        std::vector<h3d::Object*> objects;
        h3d::Camera* camera;
    };

    class Scene2D: public Scene {
    public:
        void load2D();
        void render(const h3d::Renderer &r) override;
    private:
        
        h3d::Camera c2d;
    };

} // namespace h3d


#endif