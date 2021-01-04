#ifndef HIRSCH3D_HPP
#define HIRSCH3D_HPP

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
#include "Object.hpp"




namespace h3d {

    

    class Renderer3D {
    public:
        void renderObject(const h3d::Object* o) const;

    };
    class Renderer2D {};
    class OBJLoader {};

    class Hirsch3D {

    protected:

        virtual void render(const h3d::Renderer2D&, const h3d::Renderer3D&) = 0;
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

    private:

        SDL_Window* window;
        SDL_GLContext glContext;
        OBJLoader objLoader;
        Renderer2D renderer2D;
        Renderer3D renderer3D;
        std::string title;
        uint16_t width;
        uint16_t height;
    };
}






#endif