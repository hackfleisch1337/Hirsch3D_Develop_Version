#ifndef HIRSCH3D_CAMERA_HPP
#define HIRSCH3D_CAMERA_HPP

#include "glm/glm/glm.hpp"
#include "glm/glm/ext/matrix_transform.hpp"
#include "glm/glm/gtc/matrix_transform.hpp"

namespace h3d
{
    class Camera {
    public:
        void init(float fov, float width, float height) {
            projection = glm::perspective((float) glm::radians(fov)/2.0f, width/height, 0.1f, 100.0f);
            view = glm::mat4(1.0f);
            pos = glm::vec3(0.0f);
            update();
        }

        glm::mat4 getViewProj() {
            return this->viewProj;
        }

        virtual void update() {
            viewProj = projection * view;
        }

        virtual void translate(glm::vec3 v) {
            pos += v;
            view = glm::translate(view, v * -1.0f);
        }

        glm::mat4 getView() {
            return view;
        }

        glm::vec3 getPosition() {
            return pos;
        }

protected:
        glm::vec3 pos;
        glm::mat4 projection;
        glm::mat4 view;
        glm::mat4 viewProj;

};
} // namespace h3d




#endif