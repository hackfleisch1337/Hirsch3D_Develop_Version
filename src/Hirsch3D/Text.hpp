/*#ifndef HIRSCH3D_TEXT_HPP
#define HIRSCH3D_TEXT_HPP

#define STB_TRUETYPE_IMPLEMENTATION
#include "tools/stb/stb_truetype.h"


#include "Object.hpp"
namespace h3d
{   
    
    class Text {
    public:
        
        virtual ~Text(){}
        
        void load(std::string, std::string, uint16_t);
    private:
        stbtt_bakedchar cdata[96];
        uint16_t size;
        std::string text;
        std::string path;
        GLuint fontTexture;
    };
} // namespace h3d



#endif*/