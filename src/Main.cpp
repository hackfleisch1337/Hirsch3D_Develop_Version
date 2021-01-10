#include <iostream>
#include <string>
#include <vector>
#include "Hirsch3D/Hirsch3D.hpp"


// Es war die Z-Achse!

class Game: public h3d::Hirsch3D {

private:
    h3d::Object cube;
    h3d::Camera camera;
    h3d::Scene scene1;
    h3d::Texture t;

    void setup(const h3d::OBJLoader &objLoader) override {

        // Cube vertices
        h3d::Vertex3 v_array[] = {
            h3d::Vertex3{-0.5, -0.5, -0.5,   0, 0.66f},  // 0

            h3d::Vertex3{0.5, -0.5, -0.5,   0.25,0.66},   // 1
            
            h3d::Vertex3{-0.5, 0.5, -0.5,   0.25,0.33},   // 2

            h3d::Vertex3{0.5, 0.5, -0.5,  0,0.33   },  // 3


            h3d::Vertex3{-0.5, -0.5, 0.5,   0.5,0.66},   // 4

            h3d::Vertex3{0.5, -0.5, 0.5,   0.75,0.66},    // 5
            
            h3d::Vertex3{-0.5, 0.5, 0.5, 0.5,0.33  },    // 6

            h3d::Vertex3{0.5, 0.5, 0.5,     0.75,0.33}    // 7

        };
        // Cube indices
        uint32_t indices1[] = {
                                // Vorne
                                0,1,2,
                                1,2,3,
                                // Hinten
                                4,5,6,
                                5,6,7,
                                // Unten
                                0,1,4,
                                5,4,1,
                                // Oben
                                2,3,6,
                                6,7,3,
                                // Rechts
                                1,3,5,
                                5,3,7,
                                // Links
                                0,2,4,
                                2,4,6
                                };
        
        t.load("cube.png");
        cube.load(v_array, 8, indices1, 36, h3d::color::blue, nullptr);
        std::string vertexShader = "D:\\Emanuel\\Hirsch3D\\src\\shader\\Obj1\\shader.vert";
        std::string fragmentShader = "D:\\Emanuel\\Hirsch3D\\src\\shader\\Obj1\\shader.frag";
        camera.init(100, 1280, 720);
        camera.translate({0,0,10});
        camera.update();
        scene1.load(vertexShader, fragmentShader, &camera);
        scene1.addObject(&cube);
        
        cube.rotate(0.5, {0.1, 0.1, 0.1});
    	cube.move({0.001f,0.001f, 0.f}); // 1.0018
    }
    
    void render(const h3d::Renderer &r) override {
        scene1.render(r);
        cube.rotate(0.0005, glm::vec3(0.1f, 0.1f, 0.1f));
        camera.translate({0,0,0.0});
        camera.update();
        std::cout << "Z:" << cube.position.z << std::endl;
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