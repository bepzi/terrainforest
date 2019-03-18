#pragma once

#include <cstdlib>
#include <array>

#include "vertex.hpp"

using std::array;

template <size_t N = 3>
class Plane {
   public:
    constexpr Plane() {
        size_t ii = 0;

        for (size_t y = 0; y < N; ++y) {
            for (size_t x = 0; x < N; ++x) {
                unsigned int index = (y * N) + x;

                Vertex *v = &(vertices[index]);
                v->coords = vec3(x, y, 0.0f);
                v->normal = vec3(0.0f, 0.0f, 1.0f);

                if ((x + 1) < N && (y + 1) < N) {
                    // This is a "lower left corner"

                    // Corner, +x, +x +y
                    indices[ii++] = (y * N) + x;
                    indices[ii++] = (y * N) + (x + 1);
                    indices[ii++] = ((y + 1) * N) + (x + 1);

                    // Corner, +y, +x +y
                    indices[ii++] = (y * N) + x;
                    indices[ii++] = ((y + 1) * N) + x;
                    indices[ii++] = ((y + 1) * N) + (x + 1);
                }
            }
        }
    }

    array<Vertex, N * N> vertices;
    array<unsigned int, (N - 1) * (N - 1) * 6> indices;
};
