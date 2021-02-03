#include "Hirsch3D.hpp"
#include <iostream>
#define STB_IMAGE_IMPLEMENTATION
#include "tools/stb/stb_image.h"

#define STB_TRUETYPE_IMPLEMENTATION
#include "tools/stb/stb_truetype.h"


void h3d::Hirsch3D::setTitle(std::string title) {
    this->title = title;
    SDL_SetWindowTitle(this->window, this->title.data());
}

void h3d::Hirsch3D::setSize(uint16_t width, uint16_t height) {
    this->width = width;
    this->height = height;
    SDL_SetWindowSize(this->window, this->width, this->height);
}

uint32_t h3d::Hirsch3D::getCurrentTimeMillies() {
    return (float)(clock() - startTime) / ((clock_t) 1);
}

bool h3d::Hirsch3D::init(std::string title, uint16_t width, uint16_t height, uint8_t flags) {

    this->startTime = clock();
 
    std::cout << "Hirsch3D Renderengine | Version " << YELLOW << HIRSCH3D_VERSION << RESET_CLR << std::endl
              << "**************************************" << std::endl
              << "Copyright (C) 2021 Emanuel Zache\nLicence: MIT Licence (See https://mit-license.org/)\n" 
              << "OBJ File loader by Robert Smith (Copyright (C) 2016 Robert Smith)" << std::endl
              << std::endl;


    #ifdef HIRSCH_DEBUG
        std::cout << "Launching Hirsch3D in Debug Mode" << std::endl;

    #elif defined HIRSCH_RELEASE
        std::cout << "Launching Hirsch3D in Release Mode" << std::endl;
    #endif


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
    std::cout << GREEN << "[OK] Created window (" << width << "x" << height << ")" << RESET_CLR << std::endl;
    // Glew init
    GLenum err = glewInit();
    if(err != GLEW_OK) {
        std::cout << "\033[1;31m[FAILED] Glew initialisation failed: \033[0m" << glewGetErrorString(err) << std::endl;
        return false;
    } else std::cout << "\033[1;32m[OK] Initialized Glew succesful!\033[0m" << std::endl;
    std::cout << "OpenGl Driver Version: " << YELLOW << glGetString(GL_VERSION) << RESET_CLR << std::endl;



    // Hirsch3D Texture  //
    //////////////////////
    int textureWidth = 1920;
    int textureHeight = 720;
    int bitsPerPixel = 0;
    stbi_set_flip_vertically_on_load(true);
    auto textureBuffer = stbi_load("src/Hirsch3D/md/Hirsch3D.png", &textureWidth, &textureHeight, &bitsPerPixel, 4);

    glGenTextures(1, &this->tBuffer);
    glBindTexture(GL_TEXTURE_2D, this->tBuffer);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, textureWidth, textureHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, textureBuffer);
    glBindTexture(GL_TEXTURE_2D, 0);


    if(textureBuffer) {
        stbi_image_free(textureBuffer);
    }



    hirschShader.load("D:/Emanuel/Hirsch3D/src/Hirsch3D/shader/shader.vert", "D:/Emanuel/Hirsch3D/src/Hirsch3D/shader/shader.frag");


    Vertex3 t_vertices[] = {
    /*     x    y      z    u   v    normal*/
        {-1.0, -1.0, 0.0, 0.0, 0.0,   0,0,0},
        {1.0, -1.0,  0.0, 1.0, 0.0,   0,0,0},
        {1.0,  1.0,  0.0, 1.0,  1.0,  0,0,0},
        {-1.0,  1.0,  0.0, 0.0,  1.0, 0,0,0}
    };
    uint32_t t_indices[] = {0, 1, 2, 0, 2, 3};

    titleScreen.load(t_vertices, 4, t_indices, 6, glm::vec4(1,1,1,1), nullptr);
    int textureUniformLocation = glGetUniformLocation(hirschShader.getShaderId(), "u_texture");
	if(!textureUniformLocation != -1) {
		glUniform1i(textureUniformLocation, 0);
	}
    

    ////////////

    std::cout << GREEN << "[OK] Initialized " << RESET_CLR << std::endl;

}

bool h3d::Hirsch3D::load() {
    this->setup();
    loaded = true;
    std::cout << GREEN << "[OK] Setup complete " << RESET_CLR << std::endl;
}

bool h3d::Hirsch3D::start() {
    clock_t startpoint = clock();
    // Start Mainloop
    std::cout << YELLOW << "[IN_PROCESS] Mainloop" << RESET_CLR << std::endl;
    bool close = false;

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
    
    // Mainloop
    while(!close) {

            
            glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
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


            if(!this->showTitle && this->loaded) {
                this->render(this->renderer);
            }

            if(this->showTitle && this->getCurrentTimeMillies() > (2000 + startpoint) && this->loaded) {
                this->showTitle = false;
            }

            if(this->showTitle) {
                this->hirschShader.bind();
                glActiveTexture(GL_TEXTURE0);
                glBindTexture(GL_TEXTURE_2D, this->tBuffer);
                this->renderer.renderObject(&this->titleScreen);
                glBindTexture(GL_TEXTURE_2D, 0);
                this->hirschShader.unbind();
            }



            SDL_GL_SwapWindow(window);
            SDL_Event event;
            while(SDL_PollEvent(&event)) {
                if(event.type == SDL_KEYDOWN) {
                    this->onKeyDown(event.key.keysym.sym);
                }
                if(event.type == SDL_QUIT) {
                    close = true;
                    std::cout << GREEN << "[OK] Exited Mainloop" << RESET_CLR << std::endl;
                    this->onClose();
                }
                if(event.type == SDL_KEYUP) {
                    this->onKeyUp(event.key.keysym.sym);
                }
                if(event.type == SDL_MOUSEMOTION) {
                    this->onMouseMoved(event.motion.xrel, event.motion.yrel);
                }
                
            }
            SDL_Delay(1000/this->fps);
    }

}

h3d::Hirsch3D::~Hirsch3D() {
    glDeleteTextures(1, &this->tBuffer);
    SDL_GL_DeleteContext(glContext);
    SDL_DestroyWindow(window);
    SDL_Quit();
    std::cout << GREEN << "[Terminated]" << RESET_CLR << std::endl;
}


// Renderer

void h3d::Renderer::renderObject(const h3d::Object* o) const{

    if(!o->loaded()) {
        std::cout << RED << "[FAILED] Unable to render uninitialized object" << RESET_CLR << std::endl;
        return;
    }

    o->getVertexBuffer()->bind();
    o->getIndexBuffer()->bind();
    //glDrawArrays(GL_TRIANGLE_STRIP, 0, o->getVertexBuffer()->getAmountOfVertices());
    glDrawElements(GL_TRIANGLES, o->getIndexBuffer()->getAmountOfIndices(), GL_UNSIGNED_INT, 0);
    o->getIndexBuffer()->unbind();
    o->getVertexBuffer()->unbind();
}
