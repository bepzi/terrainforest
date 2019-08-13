#include "application.hpp"
#include "node.hpp"

#include <SDL.h>
#include <glad/glad.h>

#include <chrono>

using std::chrono::duration;
using std::chrono::duration_cast;
using std::chrono::steady_clock;

int Application::run() {
    auto prev = steady_clock::now();

    Node n;

    while (running) {
        // Handle all pending events
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            handle_window_event(event);
            handle_keydown_event(event);
        }

        draw();

        auto now = steady_clock::now();
        duration<double> dt = duration_cast<duration<double>>(now - prev);
        prev = now;
        update(dt.count());
    }

    return EXIT_SUCCESS;
}

void Application::draw() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // scene.draw();
    SDL_GL_SwapWindow(window);
}

void Application::update(double dt) {
    (void)dt;
    // scene.update(dt);
}

void Application::handle_window_event(SDL_Event event) {
    if (event.type != SDL_WINDOWEVENT) {
        return;
    }

    switch (event.window.event) {
    case SDL_WINDOWEVENT_CLOSE: {
        running = false;
    } break;

    default: {
        // Do nothing with this window event
    } break;
    }
}

void Application::handle_keydown_event(SDL_Event event) {
    if (event.type != SDL_KEYDOWN) {
        return;
    }

    switch (event.key.keysym.sym) {
    case SDLK_ESCAPE: {
        running = false;
    } break;

    default: {
        // Do nothing with this key
    } break;
    }
}
