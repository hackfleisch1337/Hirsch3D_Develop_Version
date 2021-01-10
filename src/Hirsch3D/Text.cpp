/*#include "Text.hpp"
#include <string>
#include <cstdio>

void h3d::Text::load(std::string text, std::string path, uint16_t size) {

    this->text = text;
    this->path = path;
    this->size = size;

    uint8_t ttfBuffer[1<<15];
    uint8_t* tmpBitmap = new uint8_t[size*(text.size()*(size/2))];
    fread(ttfBuffer, 1, 1<<20, fopen(path.data(), "rb"));
    stbtt_BakeFontBitmap(ttfBuffer, 0, size, tmpBitmap, size, text.size()*(size/2) , 32, 96, cdata);
    
    glGenTextures(1, &fontTexture);
    glBindTexture(GL_TEXTURE_2D, fontTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, size, text.size()*(size/2), 0, GL_RGBA, GL_UNSIGNED_BYTE, tmpBitmap);

    delete tmpBitmap;
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glBindTexture(GL_TEXTURE_2D, 0);

}*/