#version 420 core

// Interpolated from the vertex shader
in vec4 worldPosition;
in vec4 viewPosition;

in vec4 normal;

out vec4 fragColor;

void main() {
    if (abs(worldPosition.x) < 0.1 && abs(worldPosition.y) < 0.1) {
        if (worldPosition.z > 0.0) {
            fragColor = vec4(0.0f, 0.0f, 1.0f, 1.0f);  // +z
        } else {
            fragColor = vec4(0.3f, 0.3f, 1.0f, 1.0f);  // -z
        }

    } else if (abs(worldPosition.z) < 0.1 && abs(worldPosition.y) < 0.1) {
        if (worldPosition.x > 0.0) {
            fragColor = vec4(1.0f, 0.0f, 0.0f, 1.0f);  // +x
        } else {
            fragColor = vec4(1.0f, 0.3f, 0.3f, 1.0f);  // -x
        }


    } else if (abs(worldPosition.z) < 0.1 && abs(worldPosition.x) < 0.1) {
        if (worldPosition.y > 0.0) {
            fragColor = vec4(0.0f, 1.0f, 0.0f, 1.0f);  // +y
        } else {
            fragColor = vec4(0.3f, 1.0f, 0.3f, 1.0f);  // -y
        }

    } else {
        fragColor = vec4(0.3f, 0.3f, 0.3f, 1.0f);
    }

    if (abs(worldPosition.x) < 0.3 &&
        abs(worldPosition.y) < 0.3 &&
        abs(worldPosition.z) < 0.3) {
        fragColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);
    }
}
