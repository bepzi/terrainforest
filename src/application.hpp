#pragma once

#include <SDL.h>

#include <memory>

class Application {
private:
    SDL_Window *window;
    bool running;

    /**
     * Draw with the OpenGL context to the window
     */
    void draw();

    /**
     * Update the application's state
     */
    void update(double dt);

    void handle_window_event(SDL_Event event);
    void handle_keydown_event(SDL_Event event);

public:
    [[gnu::nonnull]]
    explicit Application(SDL_Window *win) :
        window(win),
        running(true) {}

    /**
     * Run the main loop
     */
    [[nodiscard]]
    int run();
};
