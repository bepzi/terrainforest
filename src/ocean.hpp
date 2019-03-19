#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <unordered_map>

#include "camera.hpp"
#include "stage.hpp"

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
    size_t num_elements;

    Camera camera;

    glm::vec2 screen_size;
    glm::vec2 screen_center;

    glm::vec2 mouse_pos;
    std::unordered_map<int, std::string> pressed_keys;

    glm::mat4 model;
    glm::mat4 view;
    glm::mat4 perspective;

    void update_view_matrix();
    void update_perspective_matrix();

    // mat4 make_view_matrix();
    // mat4 make_perspective_matrix(float aspect_ratio);
};
