#ifndef HIRSCH3D_OBJECT
#define HIRSCH3D_OBJECT
#include <string>
#include "VertexBuffer.hpp"

namespace h3d
{
    class Object {
    public:
        
        virtual ~Object();
        void load(void* vertices, uint32_t amountOfVertices);
        void loadByPath();
        VertexBuffer* getVertexBuffer() const {
            return this->vertices;
        }
    private:
        h3d::VertexBuffer* vertices;
        std::string objPath;
        uint32_t amountOfVertices;
        /*
        h3d::IndexBuffer indices;
        uint32_t amountOfIndices;
        h3d::Shader shader;
        */
    };
} // namespace h3d



#endif