#version 420 core

// Interpolated from the vertex
in vec4 position;
in vec4 normal;

out vec4 fragColor;

void main() {
    fragColor = vec4(1.0f, 0.0f, 0.0f, 1.0f);
}
