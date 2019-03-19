#pragma once

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>

#include <iostream>

class Camera {
   public:
    enum Direction {
        FORWARD, BACKWARD, LEFT, RIGHT, UP, DOWN
    };

    Camera() {
        this->position = glm::vec3(0.0f);
        this->front = glm::vec3(0.0f, 0.0f, -1.0f);
        this->up = glm::vec3(0.0f, 1.0f, 0.0f);

        this->pitch = 0.0f;
        this->yaw = 0.0f;
    }

    /**
     * Create a new free-camera at a position facing an initial
     * direction, in world coordinates.
     */
    Camera(glm::vec3 position, glm::vec3 direction,
           glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f)) {
        this->position = position;
        this->front = glm::normalize(direction);
        this->up = glm::normalize(up);

        this->pitch = 0.0f;
        this->yaw = 0.0f;
    }

    /**
     * Pan the camera by some amount in X and Y.
     */
    void rotate2d(glm::vec2 dir) {
        pitch += dir.y;
        yaw += dir.x;

        // Prevent gimbal lock
        if (pitch > 89.0f) {
            pitch = 89.0f;
        } else if (pitch < -89.0f) {
            pitch = -89.0f;
        }

        glm::vec3 front;
        front.x = cos(glm::radians(pitch)) * cos(glm::radians(yaw));
        front.y = sin(glm::radians(pitch));
        front.z = cos(glm::radians(pitch)) * sin(glm::radians(yaw));
        this->front = glm::normalize(front);
    }

    /**
     * Translate the camera by some amount in world coordinates
     * relative to its current orientation.
     */
    void move(Direction direction, float amount) {
        if (direction == Direction::FORWARD) {
            position += (front * amount);
        } else if (direction == Direction::BACKWARD) {
            position -= (front * amount);
        } else if (direction == Direction::LEFT) {
            position -= glm::normalize(glm::cross(front, up)) * amount;
        } else if (direction == Direction::RIGHT) {
            position += glm::normalize(glm::cross(front, up)) * amount;
        }

        // UP and DOWN unhandled for now
    }

    /**
     * Calculate the transformation matrix for converting from world
     * coordinates into view coordinates.
     */
    glm::mat4 get_view_matrix() {
        return glm::lookAt(position, position + front, up);
    };

   private:
    // Location of the camera in world coordinates
    glm::vec3 position;

    // Where the camera is looking in world coordinates, normalized
    glm::vec3 front;

    // Perpendicular to the look direction, defines the up direction
    glm::vec3 up;

    float pitch;
    float yaw;
};
