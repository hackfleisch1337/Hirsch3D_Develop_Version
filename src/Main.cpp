#include <Hirsch3D/Hirsch3D.hpp>

class Game: public h3d::Hirsch3D {

private:
    h3d::Object hirsch, hirsch2, lightObject;

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
    h3d::Object o3;
    h3d::Texture ot;
    h3d::NormalMap on;
    h3d::RoughnessMap orougness;

    h3d::DirectionalLight l = {glm::vec3(0.0f,0.0f,1.0f), glm::vec3(1.0f,1.0f,1.0f), 1.0f};
    h3d::DirectionalLight l2 = {glm::vec3(0.0f,0.0f,1.0f), glm::vec3(0.0f,0.0f,1.0f), 1.0f};
    h3d::PointLight pl = {{-3,0,0}, {1,1,1}, 1.0f, 0.025, 0.008};
    h3d::SpotLight spl = {{0,0,7}, {0,0,1}, {1,1,1}, 1.0f,  0.99f, 0.99f};

    void setup() override {
        std::cout << "SETUP..." << std::endl;

        t.load("D:\\Emanuel\\Hirsch3dRenderEngine\\assets\\model\\hirsch\\Diffuse.jpg");
        rn.load("D:\\Emanuel\\Hirsch3dRenderEngine\\assets\\model\\hirsch\\Glossiness.png");
        nm.load("D:\\Emanuel\\Hirsch3dRenderEngine\\assets\\model\\hirsch\\Normal.jpg");

        hirsch.loadByPath("D:\\Emanuel\\Hirsch3dRenderEngine\\assets\\model\\hirsch.obj", h3d::color::cobalt_blue, nullptr, nullptr, nullptr);
        hirsch2.loadByPath("D:\\Emanuel\\Hirsch3dRenderEngine\\assets\\model\\hirsch.obj", h3d::color::cobalt_blue, nullptr, nullptr, nullptr);
        hirsch.moveInLineOfSight({-2,0,0});
        hirsch2.move({2,0,0});
        hirsch.setMaterial(glass);
        hirsch2.setMaterial(matt);

        h3d::Vertex3 vlo[] = H3D_QUAD_VERTICES(0.04,0.04);
        uint32_t ilo[] = H3D_QUAD_INDICES;
        lightObject.load(vlo, 4, ilo, 6, h3d::color::white, nullptr, nullptr, nullptr);
        lightObject.setMaterial({1,1,1,h3d::color::white, h3d::color::white});
        lightObject.moveInLineOfSight({-3,0,0});
        lightObject.rotate(45, {0,0,1});

        float width = 1.0f;
        float height = 1.0f;
        h3d::Vertex3 vo[] = {
            {-width, -height, 0.0,  0.0,  0.0,  0,0,1},
            {width, -height,  0.0,  1.0,  0.0,  0,0,1},
            {width,  height,  0.0,  1.0,  1.0,  0,0,1},
            {-width,  height, 0.0,  0.0,  1.0,  0,0,1}
        };
        uint32_t indices[] = {0,1,2, 0,2,3};
        ot.load("obj/154.JPG");
        on.load("obj/154_norm.JPG");
        orougness.load("obj/154_reflection.JPG");
        o.load(vo, 4, indices, 6, h3d::color::english_red, nullptr, &on, nullptr);
        o.setMaterial({1,1,10, h3d::color::white});
        o2.load(vo, 4, indices, 6, h3d::color::english_red, &ot, &on, nullptr);
        o2.setMaterial({1,1,10, h3d::color::white});
        o3.load(vo, 4, indices, 6, h3d::color::english_red, &ot, nullptr, nullptr);
        o3.setMaterial({1,1,10, h3d::color::white});
        o2.moveInLineOfSight({2,0,-3});
        o3.moveInLineOfSight({4,0,-3});
        o.moveInLineOfSight({0,0,-3});

    
        
        camera.initFpsCamera(100, 1280, 720);
        camera.translate({0,0,5});
        camera.update();

        scene1.load(&camera, 0.02f);

        

        //h3d::SpotLight spl = {glm::vec3(0,0,-1), glm::vec3(0,0,1), glm::vec3(0.0,1.0,0.4), 1.0f, 0.95f, 0.80f};

        //
        scene1.addObject(&o);
        scene1.addObject(&o2);
        scene1.addObject(&o3);
        scene1.addObject(&hirsch);
        scene1.addObject(&hirsch2);
        scene1.addObject(&lightObject);
        //scene1.addDirectionalLight(&l);
        scene1.addPointLight(&pl);
        //scene1.addSpotLight(&spl);
        
        w=a=s=d=space=shift=strg=alt=false;
        s2d.load2D(1280, 720);
        ts.load("Hirsch3D_t.png");
        sp.load(&ts, 520,-290, 128,72);
        s2d.addObject(&sp);
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

        pl.position.x = 5*sin(getCurrentTimeMillis()*0.0003);
        lightObject.setPosition({5*sin(getCurrentTimeMillis()*0.0003),0,0});
        //spl.position.x = 5*sin(getCurrentTimeMillis()*0.001);
        //spl.direction.x = sin(getCurrentTimeMillis() * 0.001);
        hirsch.rotate(0.15, {0.0, 1.0, 0.0});
        //o.setRotation({o.getRotation().x,70*sin(getCurrentTimeMillis()*0.001),o.getRotation().z});
        //o2.setRotation({o2.getRotation().x,70*sin(getCurrentTimeMillis()*0.001),o2.getRotation().z});
        //o3.setRotation({o2.getRotation().x,70*sin(getCurrentTimeMillis()*0.001),o2.getRotation().z});
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
    

    //
    return 0;
}
