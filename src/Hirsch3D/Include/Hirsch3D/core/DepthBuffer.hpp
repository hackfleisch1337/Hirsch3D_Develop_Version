#ifndef HIRSCH3D_DEPTH_BUFFER_HPP
#define HIRSCH3D_DEPTH_BUFFER_HPP
#include <GL/glew.h>
#include "glm/glm/glm.hpp"
#include "glm/glm/ext/matrix_transform.hpp"
#include "glm/glm/gtc/matrix_transform.hpp"
#include "Object.hpp"
#define YELLOW "\033[93m"
#define RED "\033[91m"
#define GREEN "\033[92m"
#define RESET_CLR "\x1B[0m"

namespace h3d
{
    class DepthBuffer {
    public:

        void load(glm::vec2 fbosize, glm::vec2 shadowsize);
        void bind();
        void unbind();
        glm::vec2 getShadowSize();
        GLuint getTexture();

        virtual ~DepthBuffer();

    private:
        GLuint depthMap;
        GLuint fbo;
        glm::vec2 shadowSize;

    };
} // namespace h3d



#endif