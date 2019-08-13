/**
 * main.cpp
 *
 * Entry point of the program. Sets up SDL2, GLAD, and other OpenGL
 * stuff, then kicks off the application.
 */

#include "application.hpp"

#include <SDL.h>
#include <glad/glad.h>

constexpr Uint32 ENABLED_SUBSYSTEMS = SDL_INIT_VIDEO;

constexpr auto WINDOW_TITLE = "TerrainForest";
constexpr unsigned int WINDOW_WIDTH = 640;
constexpr unsigned int WINDOW_HEIGHT = 480;
constexpr Uint32 WINDOW_FLAGS = SDL_WINDOW_OPENGL;

constexpr unsigned int GL_VERSION_MAJOR = 4;
constexpr unsigned int GL_VERSION_MINOR = 3;

constexpr Uint32 CONTEXT_FLAGS = SDL_GL_CONTEXT_DEBUG_FLAG |
                                 SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG |
                                 SDL_GL_CONTEXT_ROBUST_ACCESS_FLAG;

/**
 * Whether or not to synchronize buffer swaps with the monitor's
 * vertical refresh rate.
 */
enum VSync {
    // Do not synchronize buffer swaps with the refresh rate
    DISABLED = 0,
    // Synchronize buffer swaps with the refresh rate
    ENABLED
};

/**
 * Whether or not to draw to a separate (non-visible) buffer before
 * swapping on-screen.
 */
enum Buffering {
    // Draw directly to the on-screen buffer (faster)
    SINGLE = 0,
    // Draw to a separate buffer before swapping (slower)
    DOUBLE
};

/**
 * Whether to use hardware or software-accelerated graphics.
 */
enum Acceleration {
    // Use software-accelerated graphics (slower)
    SOFTWARE = 0,
    // Use hardware-accelerated graphics (faster)
    HARDWARE,
};

int main() {
    if (SDL_Init(ENABLED_SUBSYSTEMS) != 0) {
        SDL_LogCritical(
            SDL_LOG_CATEGORY_APPLICATION, "failed to init SDL: %s",
            SDL_GetError());
        return EXIT_FAILURE;
    }

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, GL_VERSION_MAJOR);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, GL_VERSION_MINOR);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, Buffering::DOUBLE);
    SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, Acceleration::HARDWARE);

    SDL_GL_SetAttribute(
        SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, CONTEXT_FLAGS);

    SDL_Window *window = SDL_CreateWindow(
        WINDOW_TITLE, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
        WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_FLAGS);
    if (!window) {
        SDL_LogCritical(
            SDL_LOG_CATEGORY_APPLICATION, "failed to create new SDL window: %s",
            SDL_GetError());
        SDL_Quit();
        return EXIT_FAILURE;
    }

    SDL_GLContext context = SDL_GL_CreateContext(window);
    if (!context) {
        SDL_LogCritical(
            SDL_LOG_CATEGORY_APPLICATION,
            "failed to create new OpenGL context: %s", SDL_GetError());
        SDL_Quit();
        return EXIT_FAILURE;
    }

    if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress)) {
        SDL_LogCritical(
            SDL_LOG_CATEGORY_APPLICATION, "failed to init GLAD with SDL");
        SDL_Quit();
        return EXIT_FAILURE;
    }

    SDL_LogInfo(
        SDL_LOG_CATEGORY_APPLICATION, "OpenGL Version: %s",
        glGetString(GL_VERSION));
    SDL_LogInfo(
        SDL_LOG_CATEGORY_APPLICATION, "GLSL Version: %s",
        glGetString(GL_SHADING_LANGUAGE_VERSION));
    SDL_LogInfo(
        SDL_LOG_CATEGORY_APPLICATION, "Vendor: %s", glGetString(GL_VENDOR));
    SDL_LogInfo(
        SDL_LOG_CATEGORY_APPLICATION, "Renderer: %s", glGetString(GL_RENDERER));

    if (SDL_GL_SetSwapInterval(VSync::ENABLED) != 0) {
        SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "failed to enable VSync");
    }

    glEnable(GL_DEPTH_TEST);
    glClearColor(0, 0, 0, 1);

    Application app(window);
    int exit_code = app.run();

    SDL_GL_DeleteContext(context);
    SDL_DestroyWindow(window);

    SDL_Quit();
    return exit_code;
}
