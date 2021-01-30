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

        cube.loadByPath("D:\\Emanuel\\Hirsch3dRenderEngine\\assets\\model\\hirsch.obj", h3d::color::outrageous_orange, &t);
        klumpen.loadByPath("obj/zwerg.obj", h3d::color::spring_green, nullptr);
        dummen.loadByPath("obj/dragon.obj", h3d::color::cobalt_blue, nullptr);
        dummen.moveInLineOfSight({0,-2,-15});
        klumpen.moveInLineOfSight({-1.3,0,0});
        dummen.moveInLineOfSight({1.5,0,0});

        camera.init(100, 1280, 720);
        camera.translate({0,0,5});
        camera.update();
        
        scene1.load(&camera, 0.1f);
        //scene1.load2D(8, 4.5);

        scene1.addObject(&cube);
        scene1.addObject(&klumpen);
        scene1.addObject(&dummen);


    }

    void render(const h3d::Renderer &r) override {
        scene1.render(r);
        
        cube.rotate(0.5, {1, 1, 1});
        //camera.rotate(0.2, {0.5,1,0.2});
        //camera.update();
        dummen.rotate(0.1, {0,1,0});
        dummen.moveInLineOfSight({0.1,0,0});
        klumpen.rotate(0.2, {0, 1, 2});
    }

    void onKeyPressed(SDL_KeyCode key) override {
        
    }

    void onClose() override {
        
    }

public:

};


int main(int argc, char** argv) {

    Game game;
    game.init("Hirsch3D Renderengine | OpenGL C++", 1280, 720, HIRSCH3D_NOFLAG);
    game.setFps(120);
    game.load();
    game.start();


    return 0;
}
