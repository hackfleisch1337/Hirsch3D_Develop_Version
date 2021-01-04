#ifndef HIRSCH3D_OBJECT
#define HIRSCH3D_OBJECT
#include <string>
#include "VertexBuffer.hpp"
#include "Shader.hpp"
// #include "tools/OBJ_Loader.h"

namespace h3d
{
    class Object {
    public:
        
        virtual ~Object();
        void load(void* vertices, uint32_t amountOfVertices, std::string vertexShader, std::string fragmentShader);
        void loadByPath(std::string);
        VertexBuffer* getVertexBuffer() const {
            return this->vertices;
        }
        const h3d::Shader* getShader() const {
            return &shader;
        }
    private:
        h3d::VertexBuffer* vertices;
        std::string objPath;
        uint32_t amountOfVertices;
        bool hasLoaded = false;
        /*
        h3d::IndexBuffer indices;
        uint32_t amountOfIndices;*/
        h3d::Shader shader;
        
    };
} // namespace h3d



#endif