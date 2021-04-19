#include "../Include/Hirsch3D/core/DepthBuffer.hpp"

void h3d::DepthBuffer::load(glm::vec2 fbosize, glm::vec2 shadowsize) {

    this->shadowSize = shadowsize;

    // Generate framebuffer object
    glGenFramebuffers(1, &fbo);  

    // generating texture
    glGenTextures(1, &depthMap);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, depthMap);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, 
             shadowsize.x, shadowsize.y, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);  

}

glm::vec2 h3d::DepthBuffer::getShadowSize() {
    return this->shadowSize;
}

void h3d::DepthBuffer::bind() {
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
}
void h3d::DepthBuffer::unbind() {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);  
}
GLuint h3d::DepthBuffer::getTexture() {
    return depthMap;
}

h3d::DepthBuffer::~DepthBuffer() {
    std::cout << GREEN << "[OK] Deleted depthbuffer" << RESET_CLR << std::endl;
    glDeleteTextures(1, &depthMap);
    glDeleteFramebuffers(1, &fbo);
}