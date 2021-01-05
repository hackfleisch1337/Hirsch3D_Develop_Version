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
        uint32_t indices1[] = {0,1,2};
        h3d::Vertex3 v2[6] = {
            h3d::Vertex3{-0.5f, -0.5f, 0.0f},
            h3d::Vertex3{-0.5f, 0.5f, 0.0f},
            h3d::Vertex3{0.5f, -0.5f, 0.0f},
            h3d::Vertex3{0.5f, 0.5f, 0.0f}
        };
        uint32_t indices2[] = {0,1,2,1,2,3};

        triangle.load(v_array637, 3, indices1, 3);
        rectangle.load(v2, 4, indices2, 6);
        
        scene1.load("D:\\Emanuel\\Hirsch3D\\src\\shader\\Obj1\\shader.vert", "D:\\Emanuel\\Hirsch3D\\src\\shader\\Obj1\\shader.frag");
        scene1.addObject(&triangle);
        scene1.addObject(&rectangle);
    }
    h3d::Scene scene1;
    void render(const h3d::Renderer &r) override {
        scene1.render(r);
        rectangle.rotateClockWise(0.001f, {0.2f,1,0.4f});
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