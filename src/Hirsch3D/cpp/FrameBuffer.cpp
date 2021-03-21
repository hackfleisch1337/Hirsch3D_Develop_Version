#include "../Include/Hirsch3D/core/FrameBuffer.hpp"
#include <iostream>

void h3d::FrameBuffer::load(glm::vec2 size) {
    this->size = size;
    glGenFramebuffers(1, &id);
    glGenTextures(1, &tex);
    glActiveTexture(GL_TEXTURE10);
    glBindTexture(GL_TEXTURE_2D, tex);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, (int) size.x, (int) size.y, 0, GL_RGBA8, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glBindFramebuffer(GL_FRAMEBUFFER, id);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, tex, 0);

    if( ! glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE) {
        std::cout << RED << "[FAILED] Framebuffer is not complete" << RESET_CLR << std::endl;
        return;
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glActiveTexture(GL_TEXTURE0);

    h3d::Vertex3 v[] = H3D_QUAD_VERTICES(size.x, size.y);
    uint32_t in[]= H3D_QUAD_INDICES;

    this->vertices = new h3d::VertexBuffer(v, 4);
    this->indices = new h3d::IndexBuffer(in, 6);
    this->shader.load("shader/framebuffer/framebuffer.vert", "shader/framebuffer/framebuffer.frag");

    loaded = true;
}

void h3d::FrameBuffer::bind() {
    glActiveTexture(GL_TEXTURE10);
    glBindTexture(GL_TEXTURE_2D, tex);
    glBindFramebuffer(GL_FRAMEBUFFER, id);
}

void h3d::FrameBuffer::unbind() {
    glActiveTexture(GL_TEXTURE10);
    glBindTexture(GL_TEXTURE_2D, 0);
    glActiveTexture(GL_TEXTURE0);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

h3d::FrameBuffer::~FrameBuffer() {
    delete this->vertices;
    delete this->indices;
    glDeleteTextures(1, &tex);
    glDeleteFramebuffers(1, &id);
}

glm::vec2 h3d::FrameBuffer::getSize() const {
    return this->size;
}


bool h3d::FrameBuffer::hasLoaded() const {
    return loaded;
}

GLuint h3d::FrameBuffer::getTexture() const {
    return tex;
}

void h3d::FrameBuffer::render() {
    if(!this->loaded) return;
    this->unbind();
    this->vertices->bind();    
    this->indices->bind();

    this->shader.bind();

    glActiveTexture(GL_TEXTURE10);
    glBindTexture(GL_TEXTURE_2D, tex);

    glUniform1i(glGetUniformLocation(shader.getShaderId(), "u_texture"), 10);
    glm::mat4 ma = glm::ortho(- (1280/2), 1280/2, -(720/2), 720/2);
    glUniformMatrix4fv(glGetUniformLocation(shader.getShaderId(), "u_matrix"), 1, GL_FALSE, &ma[0][0]);
    glDrawElements(GL_TRIANGLES, indices->getAmountOfIndices(), GL_UNSIGNED_INT, 0);

    glBindTexture(GL_TEXTURE_2D, 0);
    glActiveTexture(GL_TEXTURE0);

    this->shader.unbind();
    
    this->indices->unbind();
    this->vertices->unbind();
}