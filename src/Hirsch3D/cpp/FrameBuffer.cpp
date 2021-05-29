#include "../Include/Hirsch3D/core/FrameBuffer.hpp"
#include <iostream>

void h3d::FrameBuffer::load(glm::vec2 size, unsigned int components) {
    if(components > 7) {
        return;
    }
    this->size = size;
    glGenFramebuffers(1, &id);
    
    this->amountOfComponents = components;

    this->tex = new GLuint[components];

    glGenTextures(components+1, tex);

    for(int i = 0; i < components; i++) {
        glBindTexture(GL_TEXTURE_2D, tex[i]);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, size.x, size.y, 0, GL_RGBA, GL_FLOAT, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    }
    

    glBindTexture(GL_TEXTURE_2D, tex[components]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, size.x, size.y, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);

    this->bind();

    for(int i = 0; i < components; i++) {
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, tex[i], 0);
    }
    


    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, tex[components], 0);
    glDrawBuffers(components, attachments);
    this->unbind();

}

void h3d::FrameBuffer::bind() {
    glBindFramebuffer(GL_FRAMEBUFFER, id);
}

void h3d::FrameBuffer::unbind() {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

h3d::FrameBuffer::~FrameBuffer() {
    glDeleteTextures(this->amountOfComponents,tex);
    glDeleteFramebuffers(1, &id);
    delete[] tex;
}

glm::vec2 h3d::FrameBuffer::getSize() const {
    return this->size;
}


bool h3d::FrameBuffer::hasLoaded() const {
    return this->loaded;
}

GLuint h3d::FrameBuffer::getTexture() const {
   return this->tex[0];
}

GLuint h3d::FrameBuffer::getDepthBuffer() const {
    return this->tex[amountOfComponents];
}

GLuint h3d::FrameBuffer::getBrightColorBuffer() const {
    return this->tex[1];
}

GLuint h3d::FrameBuffer::getRenderTarget(unsigned int target) {
    if(target>=amountOfComponents) {
        return 0;
    }
    return this->tex[target];
}