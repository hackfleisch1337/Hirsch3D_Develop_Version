#include "../Include/Hirsch3D/core/FrameBuffer.hpp"
#include <iostream>

void h3d::FrameBuffer::load(glm::vec2 size) {
    this->size = size;
    glGenFramebuffers(1, &id);

    glGenTextures(2, tex);

    glBindTexture(GL_TEXTURE_2D, tex[0]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, size.x, size.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glBindTexture(GL_TEXTURE_2D, tex[1]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, size.x, size.y, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);

    this->bind();

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, tex[0], 0);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, tex[1], 0);

    this->unbind();

}

void h3d::FrameBuffer::bind() {
    glBindFramebuffer(GL_FRAMEBUFFER, id);
}

void h3d::FrameBuffer::unbind() {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

h3d::FrameBuffer::~FrameBuffer() {
    glDeleteFramebuffers(1, &id);
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

void h3d::FrameBuffer::render() {
 
}