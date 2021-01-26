/**
 * @file Hirsch3D/Hirsch3D.hpp
 * @ref Hirsch3d/core
 *
 * @author Emanuel Zache
 * @version 0.3.7
 * @copyright (C) 2021 Emanuel Zache
 *
 * @brief Hirsch3D is a 3D Renderengine based on OpenGl
 * Basic Setup:
 * { @code
 *  #include <Hirsch3D/Hisch3D.hpp>
 *  class Game: public h3d::Hirsch3D {
 *  private:
 *      void setup() override {}
 *      void render(const h3d::Renderer &renderer) override {}
 *      void onClose() override {};
 *  };
 *
 *  int main(int argc, char** argv) {
 *      Game g;
 *      g.init("Title", 1920, 1080, HIRSCH3D_FULLSCREEN | HIRSCH3D_BORDERLESS);
 *      g.setFps(60);
 *      g.load();
 *      g.start();
 *      return 0;
 *  }
 * }
 *
 *
 *
 *
 */


#ifndef HIRSCH3D_HPP
#define HIRSCH3D_HPP

#include <iostream>
#include <string>
#include <vector>

#define HIRSCH3D_VERSION "0.6.8"
#define YELLOW "\033[93m"
#define RED "\033[91m"
#define GREEN "\033[92m"
#define RESET_CLR "\x1B[0m"


#include <string>
#define GLEW_STATIC
#include <GL/glew.h>
#define SDL_MAIN_HANDLED
#include <SDL.h>
#include "Vertex.hpp"
#include "VertexBuffer.hpp"
#include "Texture.hpp"
#include "Object.hpp"

#include <ctime>

#include "Scene.hpp"

#include "Camera.hpp"


namespace h3d {



    class Renderer {
    public:
        void renderObject(const h3d::Object* o) const;
    };
    class OBJLoader {};

    class Hirsch3D {

    protected:

        virtual void render(const h3d::Renderer&) = 0;
        virtual void setup(const h3d::OBJLoader &objLoader) = 0;
        virtual void onClose() = 0;
        void setTitle(std::string);
        void setSize(uint16_t, uint16_t);
    public:
        #define HIRSCH3D_NOFLAG     0b00000000
        #define HIRSCH3D_FULLSCREEN 0b00000001
        #define HIRSCH3D_BORDERLESS 0b00000010
        #define HIRSCH3D_MINIMIZED  0b00000100

        bool init(std::string title, uint16_t width, uint16_t height, uint8_t flags);
        bool start();
        bool load();

        virtual ~Hirsch3D();
        void setFps(uint16_t fps) {
            if(fps > 0) {
                this->fps = fps;
            }
        }

        uint32_t getCurrentTimeMillies();

    private:
        SDL_Window* window;
        SDL_GLContext glContext;
        OBJLoader objLoader;
        Renderer renderer;
        std::string title;
        uint16_t width;
        uint16_t height;
        uint16_t fps = 60;
        GLuint tBuffer;
        h3d::Shader hirschShader;
        h3d::Object titleScreen;
        clock_t startTime;
        bool showTitle = true;
    };

    namespace color {
        #define RGB_TO_OPENGL(r,g,b) glm::vec4(r/256, g/256, b/256, 1)
        const glm::vec4 red(1.0f, 0.0f, 0.0f, 1.0f);
        const glm::vec4 green(0.0f, 1.0f, 0.0f, 1.0f);
        const glm::vec4 blue(0.0f, 0.0f, 1.0f, 1.0f);
        const glm::vec4 black(0.0f, 0.0f, 0.0f, 1.0f);
        const glm::vec4 white(1.0f, 1.0f, 1.0f, 1.0f);
        const glm::vec4 yellow(1.0f, 1.0f, 0.0f, 1.0f);
        const glm::vec4 cyan(0.0f, 1.0f, 0.0f, 1.0f);
        const glm::vec4 snow = RGB_TO_OPENGL(255.0f, 250.0f, 250.0f);
        const glm::vec4 spring_green = RGB_TO_OPENGL(94.0f, 252.0f, 141.0f);
        const glm::vec4 midnight_blue = RGB_TO_OPENGL(7.0f, 0.0f, 77.0f);
        const glm::vec4 outrageous_orange = RGB_TO_OPENGL(252.0f, 119.0f, 83.0f);
        const glm::vec4 dim_gray = RGB_TO_OPENGL(113.0f, 105.0f, 105.0f);
        const glm::vec4 celadon_green = RGB_TO_OPENGL(54.0f, 130.0f, 127.0f);
        const glm::vec4 english_violet = RGB_TO_OPENGL(89.0f, 63.0f, 98.0f);
        const glm::vec4 magic_mint = RGB_TO_OPENGL(174.0f, 255.0f, 216.0f);
        const glm::vec4 english_red = RGB_TO_OPENGL(159.0f, 74.0f, 84.0f);
        const glm::vec4 dark_purple = RGB_TO_OPENGL(28.0f, 1.0f, 24.0f);
        const glm::vec4 bone = RGB_TO_OPENGL(217.0f, 207.0f, 193.0f);
        const glm::vec4 army_green = RGB_TO_OPENGL(65.0f, 82.0f, 31.0f);
        const glm::vec4 cobalt_blue = RGB_TO_OPENGL(0.0f, 75.0f, 168.0f);
        const glm::vec4 yellow_green(0.8f, 0.61f, 0.5f, 1.0f);
    };


}






#endif
