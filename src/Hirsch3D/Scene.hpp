#ifndef HIRSCH3D_SCENE_HPP
#define HIRSCH3D_SCENE_HPP
#include <vector>
#include "Object.hpp"
#include "Hirsch3D.hpp"



namespace h3d
{
    class Renderer;
    class Scene {
    public:
        void load(std::string vertexShaderSrc, std::string fragmentShaderSrc);

        void addObject(h3d::Object* o);
        // void addLight(h3d::PointLight &p);
        void render(const h3d::Renderer &r);
        

    private:
        h3d::Shader shader;
        std::vector<h3d::Object*> objects;
    };
} // namespace h3d


#endif