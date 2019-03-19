#include <iostream>

#include "application.hpp"

int main() {
    try {
        Application::run();
    } catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
