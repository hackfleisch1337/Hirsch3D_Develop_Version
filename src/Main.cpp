#include <iostream>
#include <string>
#include <vector>
#include "Hirsch3D/Hirsch3D.hpp"

class Game: public h3d::Hirsch3D {

private:

    void setup(const h3d::OBJLoader &objLoader) override {
        std::cout << "Setup" << std::endl;
    }

    void render(const h3d::Renderer2D &r2d, const h3d::Renderer3D &r3d) override {

    }

    void onClose() override {
        std::cout << "On Close()" << std::endl;
    }

public:

};



int main(int argc, char** argv) {

    Game game;
    game.init("Mega Tolles 3D Spiel mit OpenGL. Hehe C++", 1280, 720, HIRSCH3D_NOFLAG);
    game.load();
    game.start();

    std::cout << "Moin" << std::endl;

    return 0;
}