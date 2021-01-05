#include <iostream>
#include <string>
#include <vector>
#include "Hirsch3D/Hirsch3D.hpp"

class Game: public h3d::Hirsch3D {

private:
    h3d::Object cube;
    
    void setup(const h3d::OBJLoader &objLoader) override {
        h3d::Vertex3 v_array637[] = {
            {-0.5, -0.5, 0.5, 0.0f, 0.0f},
            {0.5, -0.5,  0.5, 0.0f, 0.0f},
            {0.5,  0.5,  0.5, 0.0f, 0.0f},
            {-0.5,  0.5,  0.5, 0.0f, 0.0f},
            {-0.5, -0.5, -0.5, 0.0f, 0.0f},
            {0.5, -0.5, -0.5, 0.0f, 0.0f},
            {0.5,  0.5, -0.5, 0.0f, 0.0f},
            {-0.5,  0.5, -0.5, 0.0f, 0.0f}
        };
        uint32_t indices1[] = {
        0, 1, 2,
		2, 3, 0,
		// right
		1, 5, 6,
		6, 2, 1,
		// back
		7, 6, 5,
		5, 4, 7,
		// left
		4, 0, 3,
		3, 7, 4,
		// bottom
		4, 5, 1,
		1, 0, 4,
		// top
		3, 2, 6,
		6, 7, 3};
        
        cube.load(v_array637, 8, indices1, 36, glm::vec4(0.6f, 0.6f, 0.7f, 1.0f));
        
        scene1.load("D:\\Emanuel\\Hirsch3D\\src\\shader\\Obj1\\shader.vert", "D:\\Emanuel\\Hirsch3D\\src\\shader\\Obj1\\shader.frag");
        scene1.addObject(&cube);
    }
    h3d::Scene scene1;
    void render(const h3d::Renderer &r) override {
        scene1.render(r);
        cube.rotateClockWise(0.001, glm::vec3(0.2f, 0.5f, 0.3f));
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