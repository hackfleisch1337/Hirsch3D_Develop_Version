#ifndef HIRSCH3D_OBJECT
#define HIRSCH3D_OBJECT
#include <string>
#include "VertexBuffer.hpp"
#include "IndexBuffer.hpp"
#include "Shader.hpp"
#include "glm/glm/glm.hpp"
#include "glm/glm/ext/matrix_transform.hpp"
#include "glm/glm/gtc/matrix_transform.hpp"



// #include "tools/OBJ_Loader.h"

namespace h3d
{
    class Object {
    public:
        
        virtual ~Object();
        void load(void* vertices, uint32_t amountOfVertices, uint32_t* indices, uint32_t amountOfIndices, glm::vec4 color);
        void loadByPath(std::string, glm::vec4);
        VertexBuffer* getVertexBuffer() const {
            return this->vertices;
        }
        IndexBuffer* getIndexBuffer() const {
            return this->indices;
        }
        glm::mat4 getMatrix() {return modelMatrix;}
        void rotate(float degree, glm::vec3 direction);
        glm::vec4 color;
        bool loaded() const {return hasLoaded;}
    private:
        glm::mat4 modelMatrix;
        
        h3d::VertexBuffer* vertices;
        std::string objPath;
        uint32_t amountOfVertices;
        glm::vec3 position;
        bool hasLoaded = false;
        h3d::IndexBuffer* indices;
        uint32_t amountOfIndices;
    };
} // namespace h3d



#endif