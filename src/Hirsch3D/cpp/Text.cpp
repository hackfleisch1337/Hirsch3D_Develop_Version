#include <cstdio>
#include "../Include/Hirsch3D/core/Text.hpp"


void h3d::Text::loadText(std::string text, std::string font, uint16_t fontsize, glm::vec4 color, float x, float y) {
    std::cout << "Test" << std::endl;
    uint8_t ttfBuffer[1<<15];
    uint8_t* tmpBitmap = new uint8_t[fontsize*(fontsize/2)];

    fread(ttfBuffer, 1, 1<<15, fopen(font.data(), "rb"));
    
    stbtt_BakeFontBitmap(ttfBuffer, 0, 32.0f, tmpBitmap, fontsize, (fontsize/2), 32, 96, cdata);

    this->fontTex.loadTtf(tmpBitmap);
    delete tmpBitmap;
    
}