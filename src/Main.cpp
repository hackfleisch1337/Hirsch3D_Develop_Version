#include <Hirsch3D/Hirsch3D.hpp>

class Game: public h3d::Hirsch3D {

private:
    h3d::Object cube;
    h3d::Object klumpen;
    h3d::Object dummen;
    h3d::FpsCamera camera;
    h3d::Scene scene1;
    h3d::Texture t;

    h3d::Scene2D s2d;
    h3d::Sprite sp;
    h3d::Texture ts;

    bool w,a,s,d,space, shift, strg, alt;

    void setup() override {
        std::cout << "SET UP" << std::endl;

        t.load("D:\\Emanuel\\Hirsch3dRenderEngine\\assets\\model\\hirsch\\Diffuse.jpg");

        cube.loadByPath("D:\\Emanuel\\Hirsch3dRenderEngine\\assets\\model\\hirsch.obj", h3d::color::outrageous_orange, &t);
        klumpen.loadByPath("obj/sphere2.obj", h3d::color::english_red, nullptr);
        dummen.loadByPath("obj/dummen.obj", h3d::color::outrageous_orange, nullptr);

        dummen.moveInLineOfSight({1.5,-2,-1});
        klumpen.moveInLineOfSight({-1.3,0,0});
    

        camera.initFpsCamera(100, 1280, 720);
        camera.translate({0,0,5});
        camera.update();
        
        scene1.load(&camera, 0.01f);
        //scene1.load2D(8, 4.5);

        scene1.addObject(&cube);
        scene1.addObject(&klumpen);
        scene1.addObject(&dummen);

        w=a=s=d=space=shift=strg=alt=false;
       
        s2d.load2D(1280, 720);
        ts.load("Hirsch3D.png");
        sp.load(&ts, 410,-210, 256,144);
        s2d.addObject(&sp);

    }

    void onMouseMoved(int relX, int relY) override {
        camera.rotate(relX, relY);
    }

    void onKeyDown(SDL_Keycode key) override {
        if(key == SDLK_w) {
            w = true;
        }
        if(key == SDLK_a) {
            a = true;
        }
        if(key == SDLK_s) {
            s = true;
        }
        if(key == SDLK_d) {
            d = true;
        }
        if(key == SDLK_SPACE) {
            space = true;
        }
        if(key == SDLK_LCTRL) {
            strg = true;
        }
        if(key == SDLK_LSHIFT) {
            shift = true;
        }
        if(key == SDLK_LALT) {
            alt = true;
        }
    }

    void onKeyUp(SDL_Keycode key) override {
        if(key == SDLK_w) {
            w = false;
        }
        if(key == SDLK_a) {
            a = false;
        }
        if(key == SDLK_s) {
            s = false;
        }
        if(key == SDLK_d) {
            d = false;
        }
        if(key == SDLK_SPACE) {
            space = false;
        }
        if(key == SDLK_LCTRL) {
            strg = false;
        }
        if(key == SDLK_LSHIFT) {
            shift = false;
        }
        if(key == SDLK_LALT) {
            alt = false;
        }
    }
    float speed = 0.01;
    void render(const h3d::Renderer &r) override {
        scene1.render(r);
        //cube.rotate(0.5, {1, 1, 1});
    
        if(w) {
            camera.moveFront(speed);
        }
        if(a) {
            camera.moveSideways(-speed);
        }
        if(s) {
            camera.moveFront(-speed);
        }
        if(d) {
            camera.moveSideways(speed);
        }
        if(space) {
            camera.moveUp(speed);
        }
        if(strg) {
            camera.moveUp(-speed);
        }
        if(shift) {
            speed = 0.03;
        } else if(alt) {
            speed = 0.003;
        } else speed = 0.01;


        dummen.rotate(0.1, {0,1,0});
        dummen.moveInLineOfSight({0,0,-0.01});
        //klumpen.rotate(0.2, {0, 1, 2});
        //cube.rotate(0.09, {0, 1, 2});
        s2d.render(r);
    }

    void onClose() override {
        
    }

public:

};


int main(int argc, char** argv) {
    Game game;
    game.init("Hirsch3D Renderengine | OpenGL C++", 1920, 1080, HIRSCH3D_FULLSCREEN);
    game.setFps(180);
    game.load();
    game.start();
    
    return 0;
}
