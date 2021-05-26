#ifndef HIRSCH3D_FRAMEBUFFER_HPP
#define HIRSCH3D_FRAMEBUFFER_HPP
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


    class FrameBuffer {

    public:
        void load(glm::vec2 size, unsigned int components);
        void bind();
        void unbind();
        virtual ~FrameBuffer();
        glm::vec2 getSize() const;
        bool hasLoaded() const;
        GLuint getTexture() const;
        GLuint getDepthBuffer() const;
        GLuint getBrightColorBuffer() const;
        GLuint getRenderTarget(unsigned int target);
    private:
        GLuint id;
        GLuint* tex;
        bool loaded = false;
        glm::vec2 size;
        h3d::VertexBuffer* vertices;
        unsigned int amountOfComponents;
        h3d::IndexBuffer* indices;
        unsigned int attachments[5] = {GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3, GL_COLOR_ATTACHMENT4};
    };
} // namespace h3d


#endif