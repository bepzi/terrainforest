#include "application.hpp"

#include "ocean.hpp"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <memory>
#include <stdexcept>

static const int WIDTH = 1024;
static const int HEIGHT = 720;

// Create space for static member variables
GLFWwindow *Application::window = nullptr;
std::unique_ptr<Stage> Application::stage = nullptr;

void Application::run() {
    glfwSetErrorCallback(Application::on_glfw_error);

    if (!glfwInit()) {
        throw std::runtime_error("failed to init GLFW");
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(WIDTH, HEIGHT, "Hello World", nullptr, nullptr);
    if (!window) {
        glfwTerminate();
        throw std::runtime_error("failed to create GLFW window");
    }

    glfwMakeContextCurrent(window);
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        throw std::runtime_error("failed to init GLAD");
    }

    glEnable(GL_DEPTH_TEST);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    glfwSetKeyCallback(window, Application::on_key_event);
    glfwSetCursorPosCallback(window, Application::on_mouse_move);

    glfwSwapInterval(1);

    glViewport(0, 0, WIDTH, HEIGHT);
    glfwSetFramebufferSizeCallback(window, Application::on_window_resize);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    stage = std::make_unique<Ocean>();
    stage->init(window);

    double prev = glfwGetTime();
    while (!glfwWindowShouldClose(window)) {
        double now = glfwGetTime();
        double dt = now - prev;
        prev = now;

        stage->update(dt);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        stage->draw();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    stage->cleanup();

    glfwDestroyWindow(window);
    glfwTerminate();
}

void Application::on_mouse_move(GLFWwindow *win, double xpos, double ypos) {
    (void)win;
    stage->on_mouse_move(window, xpos, ypos);
}

void Application::on_key_event(
    GLFWwindow *win,
    int key,
    int scancode,
    int action,
    int mods) {
    (void)win;

    if (action == GLFW_PRESS) {
        if (key == GLFW_KEY_ESCAPE) {
            glfwSetWindowShouldClose(window, GLFW_TRUE);
        }
    }

    stage->on_key_event(window, key, scancode, action, mods);
}

void Application::on_window_resize(GLFWwindow *win, int width, int height) {
    (void)win;
    glViewport(0, 0, width, height);

    stage->on_window_resize(window, width, height);
}

void Application::on_glfw_error(int error, const char *description) {
    std::cerr << "GLFW: " << description << " (" << error << ")" << std::endl;
}
