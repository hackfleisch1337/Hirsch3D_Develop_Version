#include "Hirsch3D/Hirsch3D.hpp"

class Game: public h3d::Hirsch3D {

private:
    h3d::Object cube;
    h3d::Object klumpen;
    h3d::Object dummen;
    h3d::Camera camera;
    h3d::Scene scene1;
    h3d::Texture t;

    void setup(const h3d::OBJLoader &objLoader) override {


        t.load("D:\\Emanuel\\Hirsch3dRenderEngine\\assets\\model\\hirsch\\Diffuse.jpg");
        
        // Hirsch:D:\\Emanuel\\Hirsch3dRenderEngine\\assets\\model\\hirsch.obj
        cube.loadByPath("D:\\Emanuel\\Hirsch3dRenderEngine\\assets\\model\\hirsch.obj", h3d::color::outrageous_orange, &t);
        klumpen.loadByPath("obj/zwerg.obj", h3d::color::spring_green, nullptr);
        dummen.loadByPath("obj/dummen.obj", h3d::color::cobalt_blue, nullptr);
        klumpen.moveInLineOfSight({-1.3,0,0});
        dummen.moveInLineOfSight({1.5,0,0});
        //cube.move({0.5,0,0});
        std::string vertexShader = "D:\\Emanuel\\Hirsch3D\\src\\Hirsch3D\\shader\\main\\shader.vert";
        std::string fragmentShader = "D:\\Emanuel\\Hirsch3D\\src\\Hirsch3D\\shader\\main\\shader.frag";


        camera.init(100, 1280, 720);
        camera.translate({0,0,5});
        //camera.rotate(30, {1,0,0});
        camera.update();
        scene1.load(vertexShader, fragmentShader, &camera, 0.1f);

        scene1.addObject(&cube);
        scene1.addObject(&klumpen);
        scene1.addObject(&dummen);

        //cube.rotate(0.5, {0.1, 0.1, 0.1});
    	//cube.move({0.5f,0.5f, -5.f}); // bugkonstante = 1.0018
    }

    void render(const h3d::Renderer &r) override {
        scene1.render(r);
        
        cube.rotate(0.5, {1, 1, 1});
        dummen.rotate(0.9, {0,1,0});
        //dummen.moveInLineOfSight({0,0,0.05});
        klumpen.rotate(0.2, {0, 1, 2});
        //camera.rotate(1, {1,0,0});
        //camera.update();
        //cube.moveInLineOfSight({0.001,0.001,-0.02});
    }

    void onClose() override {

    }

public:

};


int main(int argc, char** argv) {

    Game game;
    game.init("Mega Tolles 3D Spiel mit OpenGL. Hehe C++", 1280, 720, HIRSCH3D_NOFLAG);
    game.setFps(120);
    game.load();
    game.start();


    return 0;
}
