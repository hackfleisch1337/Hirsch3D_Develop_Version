#ifndef HIRSCH3D_LIGHT_HPP
#define HIRSCH3D_LIGHT_HPP
#include "../../glm/glm/glm.hpp"
namespace h3d
{
    class Light {
    protected:
        glm::vec3 color;
        glm::vec3 position;
    public:
        virtual uint8_t getLightType() = 0;
        void init(glm::vec3 p, glm::vec3 c);
        void setColor(glm::vec3);
        glm::vec3 getColor();
        void setPosition(glm::vec3);
        glm::vec3 getPosition();
        void translate(glm::vec3); 
    };

    class DirectionalLight: public Light {
        uint8_t getLightType() override {
            return 0;
        }
    };

    class PointLight: public Light {
        uint8_t getLightType() override {
            return 1;
        }
    };

    class SpotLight: public Light {
        uint8_t getLightType() override {
            return 2;
        }
    };
} // namespace h3d



#endif