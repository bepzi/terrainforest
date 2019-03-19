#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <unordered_map>

#include "camera.hpp"
#include "stage.hpp"

using glm::vec3;
using glm::vec2;
using glm::mat4;

class Ocean : public Stage {
   public:
    void init(GLFWwindow *) override;

    void cleanup() override;

    void update(double dt) override;

    void draw(double dt) override;

    void on_key_event(GLFWwindow *, int, int, int, int) override;

    void on_mouse_move(GLFWwindow *, double, double) override;

    void on_window_resize(GLFWwindow *, int, int) override;

  private:
    bool wireframe = false;

    GLuint program;
    GLuint vao;

    GLuint vertex_buffer;
    GLuint index_buffer;
    GLsizei num_elements;

    Camera camera;

    vec2 screen_size;
    vec2 screen_center;

    vec2 mouse_pos;
    std::unordered_map<int, std::string> pressed_keys;

    mat4 model;
    mat4 view;
    mat4 perspective;

    void update_view_matrix();
    void update_perspective_matrix();
};
