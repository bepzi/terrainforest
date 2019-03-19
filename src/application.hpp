#pragma once

#include <memory>

#include "stage.hpp"

class Application {
public:
    static void run();

private:
    static GLFWwindow *window;

    static std::unique_ptr<Stage> stage;

    static void on_key_event(GLFWwindow *, int, int, int, int);

    static void on_mouse_event(GLFWwindow *, double, double);

    static void on_window_resize(GLFWwindow *, int, int);

    static void on_glfw_error(int, const char*);
};
