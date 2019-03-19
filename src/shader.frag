#version 420 core
#extension GL_ARB_explicit_uniform_location : enable

// Interpolated from the vertex shader
in vec3 worldPosition;
in vec4 viewPosition;
in vec3 normal;

layout(location = 5) uniform vec3 lightPos;

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
