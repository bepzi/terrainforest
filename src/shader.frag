#version 420 core

// Interpolated from the vertex shader
in vec4 worldPosition;
in vec4 viewPosition;

in vec4 normal;

out vec4 fragColor;

void main() {
    if (abs(worldPosition.x) < 0.1 && abs(worldPosition.y) < 0.1) {
        // Z axis, make it blue
        fragColor = vec4(0.0f, 0.0f, 1.0f, 1.0f);
    } else if (abs(worldPosition.z) < 0.1 && abs(worldPosition.y) < 0.1) {
        // X axis, make it red
        fragColor = vec4(1.0f, 0.0f, 0.0f, 1.0f);
    } else if (abs(worldPosition.z) < 0.1 && abs(worldPosition.x) < 0.1) {
        // Y axis, make it green
        fragColor = vec4(0.0f, 1.0f, 0.0f, 1.0f);
    } else {
        fragColor = vec4(0.3f, 0.3f, 0.3f, 1.0f);
    }

    if (abs(worldPosition.x) < 0.3 &&
        abs(worldPosition.y) < 0.3 &&
        abs(worldPosition.z) < 0.3) {
        fragColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);
    }
}
