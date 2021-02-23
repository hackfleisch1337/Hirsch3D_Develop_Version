#include <Hirsch3D/Hirsch3D.hpp>

class Game: public h3d::Hirsch3D {

private:
    h3d::Object hirsch;
    h3d::Object dummen2;
    h3d::Object dummen;

    h3d::FpsCamera camera;
    h3d::Scene scene1;
    h3d::Texture t;

    h3d::Scene2D s2d;
    h3d::Sprite sp;
    h3d::Texture ts;
    h3d::RoughnessMap rn;
    h3d::NormalMap nm;
    bool w,a,s,d,space, shift, strg, alt;

    h3d::Material matt = {1, 1, 10, h3d::color::white};
    h3d::Material glass = {12, 1, 500, h3d::color::white};
    h3d::Material hirschmaterial = {5, 1, 40};

    h3d::Object o;
    h3d::Object o2;
    h3d::Texture ot;
    h3d::NormalMap on;

    void setup() override {
        std::cout << "SETUP..." << std::endl;

        t.load("D:\\Emanuel\\Hirsch3dRenderEngine\\assets\\model\\hirsch\\Diffuse.jpg");
        rn.load("D:\\Emanuel\\Hirsch3dRenderEngine\\assets\\model\\hirsch\\Glossiness.png");
        nm.load("D:\\Emanuel\\Hirsch3dRenderEngine\\assets\\model\\hirsch\\Normal.jpg");

        hirsch.loadByPath("D:\\Emanuel\\Hirsch3dRenderEngine\\assets\\model\\hirsch.obj", h3d::color::outrageous_orange, nullptr, &nm, nullptr);
        dummen2.loadByPath("obj/dummen.obj",h3d::color::dark_gray, nullptr, nullptr, nullptr);
        dummen.loadByPath("obj/dummen.obj", h3d::color::dark_gray, nullptr, nullptr, nullptr);


        dummen.moveInLineOfSight({1.3,0,0});
        dummen2.moveInLineOfSight({-1.3,0,0});
    
        dummen.setMaterial(matt);
        dummen2.setMaterial(glass);
        hirsch.setMaterial(hirschmaterial);

        float width = 1.0f;
        float height = 1.0f;
        h3d::Vertex3 vo[] = {
             {-width, -height, 0.0,  0.0,  0.0,  0,0,1},
            {width, -height,  0.0,  1.0,  0.0,  0,0,1},
            {width,  height,  0.0,  1.0,  1.0,  0,0,1},
            {-width,  height, 0.0,  0.0,  1.0,  0,0,1}
        };
        uint32_t indices[] = {0,1,2, 0,2,3};
        ot.load("obj/188.JPG");
        on.load("obj/188_norm.JPG");
        o.load(vo, 4, indices, 6, h3d::color::english_red, &ot, &on, nullptr);
        o2.load(vo, 4, indices, 6, h3d::color::english_red, &ot, nullptr, nullptr);
        o.rotate(-30, {1,0,0});
        o.rotate(-70, {0,1,0});
        
        o2.moveInLineOfSight({2,0,0});
        o2.rotate(-30, {1,0,0});
        o2.rotate(-70, {0,1,0});
        
        camera.initFpsCamera(100, 1280, 720);
        camera.translate({0,0,5});
        camera.update();
        
        scene1.load(&camera, 0.02f);
        //scene1.addObject(&hirsch);
        //scene1.addObject(&dummen2);
        //scene1.addObject(&dummen);
        scene1.addObject(&o);
        scene1.addObject(&o2);
        w=a=s=d=space=shift=strg=alt=false;

        s2d.load2D(1280, 720);
        ts.load("Hirsch3D.png");
        sp.load(&ts, 520,-290, 128,72);
        s2d.addObject(&sp);

/*
        h3d::Vertex3 a = klumpen.getVertexBuffer()->getVertex(0);
        std::cout << a.x << " " << a.y << " " << a.z << std::endl;
        a = klumpen.getVertexBuffer()->getVertex(1);
        std::cout << a.x << " " << a.y << " " << a.z << std::endl;
        a = klumpen.getVertexBuffer()->getVertex(2);
        std::cout << a.x << " " << a.y << " " << a.z << std::endl;
        a = klumpen.getVertexBuffer()->getVertex(3);
        std::cout << a.x << " " << a.y << " " << a.z << std::endl;
        a = klumpen.getVertexBuffer()->getVertex(4);
        std::cout << a.x << " " << a.y << " " << a.z << std::endl;
        a = klumpen.getVertexBuffer()->getVertex(5);
        std::cout << a.x << " " << a.y << " " << a.z << std::endl;
        a = klumpen.getVertexBuffer()->getVertex(6);
        std::cout << a.x << " " << a.y << " " << a.z << std::endl;
        a = klumpen.getVertexBuffer()->getVertex(7);
        std::cout << a.x << " " << a.y << " " << a.z << std::endl;
*/
    }

    void onMouseMoved(int relX, int relY, glm::vec2 mouse) override {
        camera.rotate(relX, relY);
    }

    void onKeyDown(SDL_Keycode key) override {
        if(key == SDLK_w) 
            w = true;
        if(key == SDLK_a) 
            a = true;
        if(key == SDLK_s) 
            s = true;
        if(key == SDLK_d) 
            d = true;
        if(key == SDLK_SPACE) 
            space = true;
        if(key == SDLK_LCTRL) 
            strg = true;
        if(key == SDLK_LSHIFT) 
            shift = true;
        if(key == SDLK_LALT) 
            alt = true;
    }

    void onKeyUp(SDL_Keycode key) override {
        if(key == SDLK_w)
            w = false;
        if(key == SDLK_a)
            a = false;
        if(key == SDLK_s)
            s = false;
        if(key == SDLK_d)
            d = false;
        if(key == SDLK_SPACE)
            space = false;
        if(key == SDLK_LCTRL)
            strg = false;
        if(key == SDLK_LSHIFT)
            shift = false;
        if(key == SDLK_LALT)
            alt = false;
    }
    float speed = 0.01;
    void render(const h3d::Renderer &r) override {
        scene1.render(r);
        //cube.rotate(0.5, {1, 1, 1});

        if(w)
            camera.moveFront(speed);
        if(a)
            camera.moveSideways(-speed);
        if(s)
            camera.moveFront(-speed);
        if(d)
            camera.moveSideways(speed);
        if(space)
            camera.moveUp(speed);
        if(strg)
            camera.moveUp(-speed);
        if(shift) {
            speed = 0.03;
        } else if(alt) {
            speed = 0.003;
        } else speed = 0.01;


        dummen.rotate(0.13, {0,1,0});
        dummen2.rotate(0.13, {0, 1, 0});
        hirsch.rotate(0.15, {0.0, 1.0, 0.0});
        o.rotate(0.1, {0,0,1});
        o2.rotate(0.1, {0,0,1});
        s2d.render(r);        
    }

    void onClose() override {
        
    }

public:

};


int main(int argc, char** argv) {
    Game game;
    game.init("Hirsch3D Renderengine | OpenGL C++", 1280, 720, HIRSCH3D_NOFLAG);
    game.setFps(180);
    game.load();
    game.start();
    
    return 0;
}
