#include <iostream>
#include <string>
#include <vector>
#include "Hirsch3D/Hirsch3D.hpp"

class Game: public h3d::Hirsch3D {

private:
    h3d::Object triangle, rectangle;
    void setup(const h3d::OBJLoader &objLoader) override {
        h3d::Vertex3 v_array637[3] = {
            h3d::Vertex3{-0.5f, -0.5f, 0.0f},
            h3d::Vertex3{0.0f, 0.5f, 0.0f},
            h3d::Vertex3{0.5f, -0.5f, 0.0f}
        };

        h3d::Vertex3 v2[6] = {
            h3d::Vertex3{-0.5f, -0.5f, 0.0f},
            h3d::Vertex3{1.0f, 0.5f, 0.0f},
            h3d::Vertex3{0.5f, -0.5f, 0.0f},

            h3d::Vertex3{-0.5f, -0.5f, 0.0f},
            h3d::Vertex3{0.0f, 0.5f, 0.0f},
            h3d::Vertex3{1.5f, 0.5f, 0.0f}
        };

        triangle.load(v_array637, 3);
        rectangle.load(v2, 6);
        
    }

    void render(const h3d::Renderer2D &r2d, const h3d::Renderer3D &r3d) override {
        
    }

    void onClose() override {
        
    }

public:

};



int main(int argc, char** argv) {

    Game game;
    game.init("Mega Tolles 3D Spiel mit OpenGL. Hehe C++", 1280, 720, HIRSCH3D_NOFLAG);
    game.load();
    game.start();


    return 0;
}