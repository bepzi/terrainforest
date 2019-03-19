#pragma once

#include <vector>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

class Stage {
   public:
    virtual void init(GLFWwindow *) = 0;

    virtual void cleanup() = 0;

    virtual void update(double dt) = 0;

    virtual void draw(double dt) = 0;

    virtual void on_key_event(GLFWwindow *, int, int, int, int) = 0;

    virtual void on_window_resize(GLFWwindow *, int, int) = 0;

    virtual void on_mouse_move(GLFWwindow *, double, double) = 0;

    virtual ~Stage(){};
};
