#include "Keyboard.hpp"

#include <SDL.h>

bool h3d::Keyboard::isKeyDown(SDL_KeyCode key) {

    SDL_Event even;
    while(SDL_PollEvent(&event)) {
        if(event.type != SDL_KEYDOWN) {
            break;
        }
        
    }

    return false;
}