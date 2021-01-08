#ifndef HIRSCH3D_TEXTURE_HPP
#define HIRSCH3D_TEXTURE_HPP
#include <string>
#include <cinttypes>
#include <GL/glew.h>

#ifndef YELLOW
#define YELLOW "\033[93m"
#define RED "\033[91m"
#define GREEN "\033[92m"
#define RESET_CLR "\x1B[0m"
#endif

namespace h3d
{
    
    class Texture {
    public:
        void load(std::string path);
        void bind();
        void unbind();
        virtual ~Texture(){
            glDeleteTextures(1, &buffer);
        }
    private:
        std::string path;
        int32_t width;
        int32_t height;
        GLuint buffer;
        int32_t bitsPerPixel;
    };

} // namespace h3d


#endif