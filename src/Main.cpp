#include <iostream>
#include <string>
#include <vector>
#include "Hirsch3D/Hirsch3D.hpp"

class Game: public h3d::Hirsch3D {

private:
    h3d::Object cube;
    h3d::Object uml;
    h3d::Camera camera;
    h3d::Scene scene1;
    h3d::Texture t;
    void setup(const h3d::OBJLoader &objLoader) override {

        // Cube vertices
        h3d::Vertex3 v_array[] = {
            h3d::Vertex3{-0.5, -0.5, -0.5,   0,0},  // 0

            h3d::Vertex3{0.5, -0.5, -0.5,   1,0},   // 1
            
            h3d::Vertex3{-0.5, 0.5, -0.5,   0,1},   // 2

            h3d::Vertex3{0.5, 0.5, -0.5,     1,1},  // 3


            h3d::Vertex3{-0.5, -0.5, 0.5,   0,0},   // 4

            h3d::Vertex3{0.5, -0.5, 0.5,   0,0},    // 5
            
            h3d::Vertex3{-0.5, 0.5, 0.5,   0,0},    // 6

            h3d::Vertex3{0.5, 0.5, 0.5,     0,0}    // 7

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
        t.load("D:/Emanuel/Hirsch3D/UML.png");
        cube.load(v_array, 8, indices1, 36, h3d::color::yellow, nullptr);
        
        h3d::Vertex3 v2[] = {
            h3d::Vertex3{-0.14, -0.36, -0.2, 0.0,0.0},
            h3d::Vertex3{0.14, 0.36, -0.2, 1.0,1.0},  
            h3d::Vertex3{-0.14, 0.36, -0.2, 0.0,1.0},  
            h3d::Vertex3{0.14, -0.36, -0.2, 1.0,0.0}
        };
        uint32_t i2[] = {
            0,1,2,
            0,1,3
        };
        uml.load(v2, 4, i2, 6, {1,0,0,1}, &t);

        camera.init(100, 1280, 720);
        camera.translate({0,0,10});
        camera.update();
        scene1.load("D:\\Emanuel\\Hirsch3D\\src\\shader\\Obj1\\shader.vert", "D:\\Emanuel\\Hirsch3D\\src\\shader\\Obj1\\shader.frag", &camera);
        //scene1.load2D();
        scene1.addObject(&cube);
        scene1.addObject(&uml);
        
    	
    }
    
    void render(const h3d::Renderer &r) override {
        scene1.render(r);
        cube.rotate(0.0005, glm::vec3(0.1f, 0.1f, 0.1f));
        
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