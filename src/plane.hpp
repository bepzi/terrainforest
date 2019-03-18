#pragma once

#include <cstdlib>
#include <array>
#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "vertex.hpp"

using namespace glm;
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

    /**
     * Makes a matrix that puts the plane into world coordinates.
     *
     * The plane will have its center at the world origin and take up
     * 50% of world space (1/4 on each side of the origin).
     *
     * @param world_width: 1/2 the entire diameter of the world
     */
    mat4 get_model_matrix(size_t world_width = N) {
        float scale_amt = N / (float)world_width;
        float move_amt = -(float)(N - 1) / 2.0f;
        float rotate_amt = -90.0f;

        // What we want to do is translate, then rotate, then scale --
        // this is expressed backwards in matrix multiplication
        mat4 ret = glm::scale(mat4(1.0f), vec3(scale_amt));
        ret = glm::rotate(ret, radians(rotate_amt), vec3(1.0f, 0.0f, 0.0f));
        ret = glm::translate(ret, vec3(move_amt, move_amt, 0.0f));

        return ret;
    }
};
