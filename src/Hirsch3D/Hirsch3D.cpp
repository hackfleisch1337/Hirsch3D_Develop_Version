#include "Hirsch3D.hpp"
#include <iostream>


void h3d::Hirsch3D::setTitle(std::string title) {
    this->title = title;
    SDL_SetWindowTitle(this->window, this->title.data());
}
void h3d::Hirsch3D::setSize(uint16_t width, uint16_t height) {
    this->width = width;
    this->height = height;
    SDL_SetWindowSize(this->window, this->width, this->height);
}
bool h3d::Hirsch3D::init(std::string title, uint16_t width, uint16_t height, uint8_t flags) {
    
    // Basic init
    this->title = title;
    this->height = height;
    this->width = width;

    // SDL init
    SDL_Init(SDL_INIT_EVERYTHING);
    SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_BUFFER_SIZE, 32);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1); // Use Doublebuffer
    SDL_GL_SetSwapInterval(-1);
    SDL_SetRelativeMouseMode(SDL_TRUE);
    uint32_t sdl_flags = SDL_WINDOW_OPENGL;
    
    if (flags != 0x00) {
        if((~HIRSCH3D_FULLSCREEN ) | flags == 0xFF) {
            sdl_flags = sdl_flags | SDL_WINDOW_FULLSCREEN;
        }
        if((~HIRSCH3D_BORDERLESS ) | flags == 0xFF) {
            sdl_flags = sdl_flags | SDL_WINDOW_BORDERLESS;
        }
        if((~HIRSCH3D_MINIMIZED ) | flags == 0xFF) {
            sdl_flags = sdl_flags | SDL_WINDOW_MINIMIZED;
        }
    }


    this->window = SDL_CreateWindow(this->title.data(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, sdl_flags);
    this->glContext = SDL_GL_CreateContext(window);
    // Glew init
    GLenum err = glewInit();
    if(err != GLEW_OK) {
        std::cout << "\033[1;31m[FAILED] Glew initialisation failed: \033[0m" << glewGetErrorString(err) << std::endl;
        return false;
    } else std::cout << "\033[1;32m[OK] Initialized Glew succesful!\033[0m" << std::endl;
    std::cout << "OpenGl Driver Version: " << YELLOW << glGetString(GL_VERSION) << RESET_CLR << std::endl;
    
    std::cout << GREEN << "[OK] Initialized " << RESET_CLR << std::endl;

}

bool h3d::Hirsch3D::load() {
    this->setup(this->objLoader);
    std::cout << GREEN << "[OK] Setup complete " << RESET_CLR << std::endl;
}

bool h3d::Hirsch3D::start() {

    // Start Mainloop
    std::cout << YELLOW << "[IN_PROCESS] Mainloop" << RESET_CLR << std::endl;
    bool close = false;
    
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    //glEnable(GL_CULL_FACE);
    //glEnable(GL_DEPTH_TEST)
    // Mainloop
    while(!close) {
            
            
            glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            
            
            /*############ GL DRAW ##################
            buffer.bind();
            glDrawArrays(GL_TRIANGLES, 0, buffer.getAmountOfVertices());
            buffer.unbind();

            
            glBegin(GL_TRIANGLES);


            glVertex2f(-0.5f, -0.5f);
            glVertex2f(0.0f, 0.5f);
            glVertex2f(0.5f, -0.5f);

            glEnd();
            /*############ GL END ###############################*/

            

            this->render(this->renderer);

            SDL_GL_SwapWindow(window);
            SDL_Event event;
            while(SDL_PollEvent(&event)) {
                if(event.type == SDL_QUIT) {
                    close = true;
                    std::cout << GREEN << "[OK] Exited Mainloop" << RESET_CLR << std::endl;
                    this->onClose();
                } 
            }
            SDL_Delay(1000/this->fps);
    }

}

h3d::Hirsch3D::~Hirsch3D() {
    SDL_GL_DeleteContext(glContext);
    SDL_DestroyWindow(window);
    SDL_Quit();
    std::cout << GREEN << "[Terminated]" << RESET_CLR << std::endl;
}


// Renderer

void h3d::Renderer::renderObject(const h3d::Object* o) const{
    
    o->getVertexBuffer()->bind();
    o->getIndexBuffer()->bind();
    //glDrawArrays(GL_TRIANGLE_STRIP, 0, o->getVertexBuffer()->getAmountOfVertices());
    glDrawElements(GL_TRIANGLES, o->getIndexBuffer()->getAmountOfIndices(), GL_UNSIGNED_INT, 0);
    o->getIndexBuffer()->unbind();
    o->getVertexBuffer()->unbind();
    
}