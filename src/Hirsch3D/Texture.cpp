#include "Texture.hpp"
#include "tools/stb/stb_image.h"

void h3d::Texture::load(std::string path) {
    stbi_set_flip_vertically_on_load(true);
    this->path = path;
    auto texBuffer = stbi_load(path.data(), &this->width, &this->height, &this->bitsPerPixel, 4);

    glGenTextures(1, &this->buffer);
    glBindTexture(GL_TEXTURE_2D, this->buffer);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, this->width, this->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, texBuffer);
    glBindTexture(GL_TEXTURE_2D, 0);

    if(texBuffer) {
        stbi_image_free(texBuffer);
    } else {
        std::cout << RED << "[FAILED] Failes loading" << RESET_CLR << std::endl;
    }
}

void h3d::Texture::bind() {
    glBindTexture(GL_TEXTURE_2D, this->buffer);
}

void h3d::Texture::unbind() {
    glBindTexture(GL_TEXTURE_2D, 0);
}