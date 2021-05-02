/**
 * Hirsch3D Renderengine v.0.10.1
 * For more informations see README.md
 * or read the documentation in the doc folder
 * GitHub Repository: https://github.com/hackfleisch1337/Hirsch3D_Release_v1.0
 * Licence: MIT Licence
 * Last changes: 02.03.2021 (DD.MM.YYYY)
 * 
 * @file Hirsch3D/core/Light.hpp
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


#ifndef HIRSCH3D_LIGHT_HPP
#define HIRSCH3D_LIGHT_HPP
#include "../../glm/glm/glm.hpp"
#define MAX_LIGHTS_ 10

namespace h3d
{

 
    #define H3D_SHADOWMAP_NONE -1
    #define H3D_SHADOWMAP_0     0
    #define H3D_SHADOWMAP_1     1
    #define H3D_SHADOWMAP_2     2
    #define H3D_SHADOWMAP_3     3


    /**
     * A light which shines in a direction
     * 
     */
    struct DirectionalLight {
        glm::vec3 direction;
        glm::vec3 color;
        float brightness;
        int8_t shadowMap = H3D_SHADOWMAP_NONE;
    };


    /**
     * A light which shines from a certain point
     * 
     */
    struct PointLight {
        glm::vec3 position;
        glm::vec3 color;
        float brightness;
        float linear;
        float quadratic;

    };

    /**
     * A light which shines from a certain point into
     * a certain direction in a certian angle.
     * 
     */
    struct SpotLight {
        glm::vec3 position;
        glm::vec3 direction;
        glm::vec3 color;
        float brightness;
        float innerCone;
        float outerCone;
    };


} // namespace h3d



#endif