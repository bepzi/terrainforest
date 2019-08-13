#pragma once

#include <memory>
#include <optional>
#include <vector>

#include <glm/glm.hpp>

using glm::mat4;
using glm::vec3;

class Node {
private:
    std::optional<std::unique_ptr<Node>> parent;
    std::vector<std::unique_ptr<Node>> children;

    vec3 position;
    float pitch;
    float yaw;
    float roll;

    mat4 transform;

public:
    explicit Node() :
        parent(std::nullopt),
        children(std::vector<std::unique_ptr<Node>>()),
        position(vec3(0.0, 0.0, 0.0)),
        pitch(0.0),
        yaw(0.0),
        roll(0.0),
        transform(mat4(1.0)) {}
};
