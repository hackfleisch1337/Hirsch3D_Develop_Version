#include "../Include/Hirsch3D/core/Camera.hpp"

void h3d::Camera::init(float fov, float width, float height) {
    projection = glm::perspective((float) glm::radians(fov)/2.0f, width/height, 0.01f, 1000.0f);
    view = glm::mat4(1.0f);
    pos = glm::vec3(0.0f);
    initialized = true;
    update();
}

  
void h3d::Camera::init(float viewSpaceWidth, float viewSpaceHeight) {
    projection = glm::ortho(- (viewSpaceWidth/2), viewSpaceWidth/2, -(viewSpaceHeight/2), viewSpaceHeight/2);
    view = glm::mat4(1.0f);
    pos = glm::vec3(0.0f);
    initialized = true;
    update();
}

glm::mat4 h3d::Camera::getViewProj() {
    return this->viewProj;
}

        
void h3d::Camera::update() {
    if(!initialized) return;
    viewProj = projection * view;
}

     
void h3d::Camera::translate(glm::vec3 v) {
    if(!initialized) return;
    pos += v;
    view = glm::translate(view, v * -1.0f);
}

    
void h3d::Camera::rotate(float degree, glm::vec3 d) {
    if(!initialized) return;
    this->view = glm::rotate(this->view, glm::radians(degree), d);
}

glm::mat4 h3d::Camera::getView() {
    return view;
}

glm::vec3 h3d::Camera::getPosition() {
    return pos;
}


void h3d::FpsCamera::initFpsCamera(float fov, float width, float height) {
    this->init(fov, width, height);
    up = glm::vec3(0.0f,1.0f,0.0f);
    yaw = -90.0f;
    pitch = 0.0f;
    initialized = true;
    rotate(0.0f, 0.0f);
    update();
}

void h3d::FpsCamera::rotate(float xRel, float yRel) {
    if(!initialized) return;
    yaw += xRel * mouseSensitivityX;
    pitch -= yRel * mouseSensitivityY;
    if(pitch > 89.0f)
        pitch = 89.0f;
    if(pitch < -89.0f)
        pitch = -89.0f;
    glm::vec3 front;
    front.x = cos(glm::radians(pitch)) * cos(glm::radians(yaw));
    front.y = sin(glm::radians(pitch));
    front.z = cos(glm::radians(pitch)) * sin(glm::radians(yaw));
    lookAt = glm::normalize(front);
    update();
}

void h3d::FpsCamera::update() {
    if(!initialized) return;
    view = glm::lookAt(pos, pos + lookAt, up);
    viewProj = projection * view;
}

void h3d::FpsCamera::moveFront(float amount) {
    if(!initialized) return;
    translate(glm::normalize(glm::vec3(1.0f, 0.0f, 1.0f)*lookAt) * amount);
    update();
}

void h3d::FpsCamera::moveSideways(float amount) {
    if(!initialized) return;
    translate(glm::normalize(glm::cross(lookAt, up)) * amount);
    update();
}

void h3d::FpsCamera::moveUp(float amount) {
    if(!initialized) return;
    translate(glm::normalize(glm::vec3(0.0f, 1.0f, 0.0f)) * amount);
    update();
}

glm::mat4 h3d::Camera::getProj() {
    return this->projection;
}