#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>

#include <memory>
#include <unordered_map>

#include "stage.hpp"

using namespace glm;

using std::unique_ptr;
using std::array;

class Ocean : public Stage {
   public:
    void init();

    void cleanup();

    void update(double dt);

    void draw(double dt);

    void on_key_event(GLFWwindow *, int, int, int, int);

    void on_window_resize(GLFWwindow *, int, int);

  private:
    bool wireframe = false;

    GLuint program;
    GLuint vao;

    GLuint vertex_buffer;
    GLuint index_buffer;

    size_t num_elements;

    struct Camera {
        // In world coordinates ([-width, width] in all axis)
        vec3 position;
        // In world or view coordinates, always unit-length
        quat orientation;
    } camera;

    std::unordered_map<int, std::string> pressed_keys;

    mat4 model;
    mat4 view;
    mat4 perspective;

    mat4 make_view_matrix();
    mat4 make_perspective_matrix(float aspect_ratio);
};
