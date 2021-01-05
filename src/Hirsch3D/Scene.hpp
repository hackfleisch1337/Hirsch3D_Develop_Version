#ifndef HIRSCH3D_SCENE_HPP
#define HIRSCH3D_SCENE_HPP
#include <vector>
#include "Object.hpp"
#include "Hirsch3D.hpp"

namespace h3d
{
    class Scene {
    public:
        Scene(h3d::Shader &shader);

        void addObject(h3d::Object* o);
        // void addLight(h3d::PointLight &p);
        void render(h3d::Renderer &r);

    private:
        bool isActive;
        std::vector<h3d::Object*> objects;
    };
} // namespace h3d


#endif