#include <iostream>
#include <memory>
#include <stdexcept>

#include "application.hpp"
#include "ocean.hpp"

static const int WIDTH = 640;
static const int HEIGHT = 480;

// Create space for static member variables
GLFWwindow * Application::window = nullptr;
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

    // TODO: https://stackoverflow.com/a/41273081
    // https://gafferongames.com/post/fix_your_timestep/
    double prev = glfwGetTime();
    while (!glfwWindowShouldClose(window)) {
        double now = glfwGetTime();
        double dt = now - prev;
        prev = now;

        stage->update(dt);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        stage->draw(dt);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    stage->cleanup();

    glfwDestroyWindow(window);
    glfwTerminate();
}

void Application::on_mouse_move(GLFWwindow *window, double xpos, double ypos) {
    stage->on_mouse_move(window, xpos, ypos);
}

void Application::on_key_event(GLFWwindow *window, int key,
                               int scancode, int action, int mods) {
    if (action == GLFW_PRESS) {
        if (key == GLFW_KEY_ESCAPE) {
            glfwSetWindowShouldClose(window, GLFW_TRUE);
        }
    }

    stage->on_key_event(window, key, scancode, action, mods);
}

void Application::on_window_resize(GLFWwindow *window, int width, int height) {
    (void)window;
    glViewport(0, 0, width, height);

    stage->on_window_resize(window, width, height);
}

void Application::on_glfw_error(int error, const char *description) {
    std::cerr << "GLFW: " << description
              << " (" << error << ")" << std::endl;
}
