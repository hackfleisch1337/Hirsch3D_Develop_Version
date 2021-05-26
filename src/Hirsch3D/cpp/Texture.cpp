#include "../Include/Hirsch3D/core/Texture.hpp"
#include "../Include/stb/stb_image.h"
#include "../Include/Hirsch3D/error/Exception.hpp"

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
        std::cout << GREEN << "[Ok] Loaded Texture " << YELLOW << "(" << path << ")" << RESET_CLR << std::endl;
        hasLoaded = true;
    } else {
        throw h3d::Exception("Texture could not be loaded");
    }
}

void h3d::Texture::loadTtf(uint8_t* ttfBuffer) {
    this->path = "///TTF///";
    glGenTextures(1, &this->buffer);
    glBindTexture(GL_TEXTURE_2D, this->buffer);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, this->width, this->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, ttfBuffer);
    glBindTexture(GL_TEXTURE_2D, 0);
    hasLoaded = true;
}

void h3d::Texture::bind() {
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, this->buffer);
}

void h3d::Texture::unbind() {
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void h3d::NormalMap::bind() {
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, this->buffer);
}

void h3d::RoughnessMap::bind() {
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, this->buffer);
}

void h3d::NormalMap::unbind() {
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void h3d::RoughnessMap::unbind() {
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, 0);
}


void h3d::MetallicMap::bind() {
    glActiveTexture(GL_TEXTURE4);
    glBindTexture(GL_TEXTURE_2D, this->buffer);
}

void h3d::MetallicMap::unbind() {
    glActiveTexture(GL_TEXTURE4);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void h3d::CubeMap::bind() {
    glActiveTexture(GL_TEXTURE3);
    glBindTexture(GL_TEXTURE_CUBE_MAP, this->buffer);
}

void h3d::CubeMap::unbind() {
    glActiveTexture(GL_TEXTURE3);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void h3d::CubeMap::loadCubeMap(std::string px, std::string py, std::string pz,std::string nx, std::string ny, std::string nz) {
    
    glActiveTexture(GL_TEXTURE3);
    glGenTextures(1, &this->buffer);
    glBindTexture(GL_TEXTURE_CUBE_MAP, this->buffer);

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    
    std::string* files[] = {&px, &nx, &py, &ny, &pz, &nz};
    stbi_set_flip_vertically_on_load(false);
    for(int i = 0; i < 6; i++) {
        
        auto texBuffer = stbi_load(files[i]->data(), &this->width, &this->height, &this->bitsPerPixel, 4);
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X+i, 0, GL_RGBA8, this->width, this->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, texBuffer);
        if(texBuffer) {
            stbi_image_free(texBuffer);
            std::cout << GREEN << "[Ok] Loaded Cubemap Texture " << YELLOW << "[" << i << "]" << RESET_CLR << std::endl;
        } else {
            throw h3d::Exception("Texture could not be loaded");
        }
    }
    hasLoaded = true;

    glBindTexture(GL_TEXTURE_2D, 0);
    glActiveTexture(GL_TEXTURE0);
}