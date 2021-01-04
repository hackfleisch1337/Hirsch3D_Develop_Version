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

        triangle.load(v_array637, 3, "D:\\Emanuel\\Hirsch3D\\src\\shader\\Obj1\\shader.vert", "D:\\Emanuel\\Hirsch3D\\src\\shader\\Obj1\\shader.frag");
        rectangle.load(v2, 6, "D:\\Emanuel\\Hirsch3D\\src\\shader\\Obj1\\shader.vert", "D:\\Emanuel\\Hirsch3D\\src\\shader\\Obj1\\shader.frag");
        
    }

    void render(const h3d::Renderer &r) override {
        r.renderObject(&triangle);
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