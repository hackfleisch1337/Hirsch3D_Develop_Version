#ifndef HIRSCH3D_KEYBOARD_HPP
#define HIRSCH3D_KEYBOARD_HPP


namespace h3d {

    class Keyboard {

        int keys[1024];

        void checkForKeyDown();
        void checkForKeyUp();
        bool isKeyDown(SDL_KeyCode key);

    };


}; // namespace h3d

#endif