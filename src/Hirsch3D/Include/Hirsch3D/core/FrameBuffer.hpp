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
        void load(glm::vec2 size);
        void bind();
        void unbind();
        virtual ~FrameBuffer();
        glm::vec2 getSize() const;
        bool hasLoaded() const;
        GLuint getTexture() const;
        void render();
    private:
        GLuint id;
        GLuint tex;
        bool loaded = false;
        glm::vec2 size;
        h3d::VertexBuffer* vertices;
        h3d::IndexBuffer* indices;
        h3d::Shader shader;
        
    };
} // namespace h3d


#endif