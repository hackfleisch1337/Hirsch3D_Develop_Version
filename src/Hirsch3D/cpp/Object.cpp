#include "../Include/Hirsch3D/core/Object.hpp"
#include "../Include/OBJ_Loader.h"
#include <cmath>
#ifndef GREEN
#define GREEN "\033[92m"
#define RESET_CLR "\x1B[0m"
#define RED "\033[91m"
#endif

h3d::Object::~Object() {
    if(hasLoaded) {
        delete this->vertices;
        delete this->indices;
        std::cout << GREEN  << "[OK] Deleted Object " << YELLOW << "(" << this->objPath << ")" << RESET_CLR << std::endl;
    }
}

void h3d::Object::load  (void* vertices, uint32_t amountOfVertices, uint32_t* indices, uint32_t amountOfIndices, glm::vec4 color,
                        h3d::Texture* texture, h3d::NormalMap* nmap, h3d::RoughnessMap* r) {
    this->amountOfVertices = amountOfVertices;
    this->vertices = new h3d::VertexBuffer(vertices, amountOfVertices);
    this->indices = new h3d::IndexBuffer(indices, amountOfIndices);
    this->modelMatrix = glm::mat4(1.0f);
    this->color = color;
    this->position.x = 0;
    this->position.y = 0;
    this->position.z = 0;
    this->texture = texture;
    this->nmap = nmap;
    this->rm = r;
    this->rotationVector = glm::vec3(0.0f,0.0f,0.0f);
    this->material = {1.0f, 1.0f, 10.0f};
    this->hasLoaded = true;
    std::cout << GREEN << "[Ok] Loaded 3D Object " << YELLOW << "(" << this->objPath << ", " << amountOfVertices << " vertices, "
    << amountOfIndices << " indices)" << RESET_CLR << std::endl;
}

void h3d::Object::loadByPath(std::string path, glm::vec4 color, h3d::Texture* texture, h3d::NormalMap* nm, h3d::RoughnessMap* r) {
    this->objPath = path;
    objl::Loader loader;
    if(!loader.LoadFile(path)) {
        std::cout << "\033[1;31m[FAILED] Failed to load Obj: " << path << "!\033[0m" << std::endl;
            return;
    }

    objl::Mesh c = loader.LoadedMeshes[0]; // TODO
    std::vector<h3d::Vertex3> l_vertices;
    for(int i = 0; i < c.Vertices.size(); i++) {
        l_vertices.push_back({  c.Vertices.at(i).Position.X,
                                c.Vertices.at(i).Position.Y,
                                c.Vertices.at(i).Position.Z,
                                c.Vertices.at(i).TextureCoordinate.X,
                                c.Vertices.at(i).TextureCoordinate.Y,
                                c.Vertices.at(i).Normal.X,
                                c.Vertices.at(i).Normal.Y,
                                c.Vertices.at(i).Normal.Z
                            });
    }
    //std::cout << path << "_OBJLOADER::__" << l_vertices.data()[0].x << " " << l_vertices.data()[0].y << " " << l_vertices.data()[0].z << std::endl;
    
    this->load(l_vertices.data(), c.Vertices.size(), c.Indices.data(), c.Indices.size(), color, texture, nm, r);
}

void h3d::Object::scale(float s) {
    this->modelMatrix = glm::scale(this->modelMatrix, {s,s,s});
    scaleVector = glm::vec3(s);
}

void h3d::Object::scale(glm::vec3 s) {
    this->modelMatrix = glm::scale(this->modelMatrix, s);
    scaleVector = s;
}

glm::vec3 h3d::Object::getScale() {
    return scaleVector;
}

void h3d::Object::move(glm::vec3 d) {
    glm::vec3 newPosition = this->position + d;
    this->setPosition(newPosition);
}

void h3d::Object::moveInLineOfSight(glm::vec3 d) {
    this->modelMatrix = glm::translate(this->modelMatrix, d);
}

glm::vec3 h3d::Object::getPosition() {
    return this->position;
}

void h3d::Object::setPosition(glm::vec3 pos) {
        this->modelMatrix = glm::mat4(1.0f);
        glm::vec3 currentRotation = this->rotationVector;
        this->rotationVector = glm::vec3(0.0f);
        this->moveInLineOfSight(pos);
        this->setRotation(currentRotation);
        this->scale(scaleVector);
        this->position = pos;
}

void h3d::Object::rotate(float degree, glm::vec3 direction) {
    this->modelMatrix = glm::rotate(this->modelMatrix, glm::radians(degree), glm::normalize(direction));
    this->rotationVector += glm::normalize(direction) * degree;
    
}

void h3d::Object::setRotation(glm::vec3 r) {
    float x = r.x - this->rotationVector.x;
    float y = r.y - this->rotationVector.y;
    float z = r.z - this->rotationVector.z;
    this->rotate(x,{1,0,0});
    this->rotate(y,{0,1,0});
    this->rotate(z,{0,0,1});
}

void h3d::Object::setShadowMode(bool castShadow, bool catchShadow) {
    this->castShadow = castShadow;
    this->catchShadow = catchShadow;
}

void h3d::Sprite::load(h3d::Texture* t, float x, float y, float w, float h) {

    float width, height;
    if(w <= 0)
        width = static_cast<float> (t->getWidth()) / 2;
    else width = w;
    if(h <= 0)
        height = static_cast<float> (t->getHeight()) / 2;
    else height = h;

    h3d::Vertex3 v[] = {
    /*     x    y           z    u     v   normal*/
        {-width, -height, 0.0,  0.0,  0.0,  0,0,0},
        {width, -height,  0.0,  1.0,  0.0,  0,0,0},
        {width,  height,  0.0,  1.0,  1.0,  0,0,0},
        {-width,  height, 0.0,  0.0,  1.0,  0,0,0}
    };
    uint32_t indices[] = {0,1,2, 0,2,3};
    Object::load(v, 4, indices, 6, glm::vec4(1.0f,1.0f,1.0f,1.0f), t, nullptr, nullptr);
    this->move(x,y);
}

void h3d::Sprite::move(float x, float y) {
    Object::moveInLineOfSight({x,y,0});
}

void h3d::Sprite::rotate(float degree) {
    Object::rotate(degree, {0,0,1});
}

void h3d::Object::setMaterial(h3d::Material m) {
    this->material = m;
}


void calculateTangents(h3d::Vertex3* vertices, uint32_t amountOfVertices, uint32_t* indices, uint32_t amountOfIndices) {
    for(int i = 0; i < amountOfIndices/3;i+=3) {
        h3d::Vertex3 a = vertices[indices[i]];
        h3d::Vertex3 b = vertices[indices[i+1]];
        h3d::Vertex3 c = vertices[indices[i+2]];
    }
}

void h3d::Object::setTransparent(bool t) {
    this->isTransparent = t;
}

bool h3d::Object::getTransparency() {
    return this->isTransparent;
}

void h3d::Object::setVisible(bool v) {
    this->draw = v;
}

bool h3d::Object::isVisible() {
    return this->draw;
}

void h3d::Object::setReflectionCubeMap(h3d::CubeMap* cubemap) {
    this->reflectionCubeMap = cubemap;
}