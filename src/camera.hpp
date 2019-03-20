#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

using glm::vec2;
using glm::vec3;

using glm::acos;
using glm::asin;
using glm::cos;
using glm::sin;

#include <iostream>

class Camera {
public:
    enum Direction { FORWARD, BACKWARD, LEFT, RIGHT, UP, DOWN };

    static constexpr float MAX_PITCH = 89.0f;
    static constexpr float MIN_PITCH = -89.0f;

    /**
     * Create a new free-camera at a position facing an initial
     * direction, in world coordinates.
     */
    Camera(vec3 pos, vec3 direction, vec3 upwards) :
        position(pos),
        front(glm::normalize(direction)),
        up(glm::normalize(upwards)) {

        float pitch_rad = asin(front.y);

        if (glm::degrees(pitch_rad) > MAX_PITCH ||
            glm::degrees(pitch_rad) < MIN_PITCH) {
            std::string err_msg =
                "cannot create a camera with pitch not within range";
            err_msg.append(" [");
            err_msg.append(std::to_string(MIN_PITCH));
            err_msg.append(", ");
            err_msg.append(std::to_string(MAX_PITCH));
            err_msg.append("], but got: ");
            err_msg.append(std::to_string(glm::degrees(pitch_rad)));
            err_msg.append(" degrees");

            throw std::invalid_argument(err_msg);
        }

        // Division by zero shouldn't be possible here
        float yaw_rad = asin(front.z / cos(pitch_rad));

        this->pitch = glm::degrees(pitch_rad);
        this->yaw = glm::degrees(yaw_rad);
    }

    Camera() :
        Camera(
            vec3(0.0f, 0.0f, 0.0f),
            vec3(0.0f, 0.0f, -1.0f),
            vec3(0.0f, 1.0f, 0.0f)) {}

    /**
     * Pan the camera by some amount in X and Y.
     */
    void rotate2d(glm::vec2 dir) {
        pitch += dir.y;
        yaw += dir.x;

        // Prevent gimbal lock
        if (pitch > MAX_PITCH) {
            pitch = MAX_PITCH;
        } else if (pitch < MIN_PITCH) {
            pitch = MIN_PITCH;
        }

        this->front.x =
            (float)(cos(glm::radians(pitch)) * cos(glm::radians(yaw)));
        this->front.y = (float)(sin(glm::radians(pitch)));
        this->front.z =
            (float)(cos(glm::radians(pitch)) * sin(glm::radians(yaw)));
        this->front = glm::normalize(front);
    }

    /**
     * Translate the camera by some amount in world coordinates
     * relative to its current orientation.
     */
    void move(Direction direction, float amount) {
        switch (direction) {
        case Direction::FORWARD:
            position += (front * amount);
            break;
        case Direction::BACKWARD:
            position -= (front * amount);
            break;
        case Direction::LEFT:
            position -= glm::normalize(glm::cross(front, up)) * amount;
            break;
        case Direction::RIGHT:
            position += glm::normalize(glm::cross(front, up)) * amount;
            break;
        case Direction::UP:
            position += (up * amount);
            break;
        case Direction::DOWN:
            position -= (up * amount);
            break;
        }
    }

    /**
     * Calculate the transformation matrix for converting from world
     * coordinates into view coordinates.
     */
    glm::mat4 get_view_matrix() {
        return glm::lookAt(position, position + front, up);
    }

    glm::vec3 get_position() {
        return this->position;
    }

private:
    // Location of the camera in world coordinates
    glm::vec3 position;

    // Where the camera is looking in world coordinates, normalized
    glm::vec3 front;

    // Perpendicular to the look direction, defines the up direction
    glm::vec3 up;

    // Pitch and yaw, in degrees
    float pitch;
    float yaw;
};
