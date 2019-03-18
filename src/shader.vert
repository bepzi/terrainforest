#version 420 core
#extension GL_ARB_explicit_uniform_location : enable

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNorm;

layout(location = 2) uniform mat4 uModel;
layout(location = 3) uniform mat4 uView;
layout(location = 4) uniform mat4 uPersp;

// layout(location = 3) uniform mat4 uModelInvTransp;

out vec4 position;
out vec4 normal;

void main() {
    position = uPersp * uView * uModel * vec4(aPos, 1.0);
    // normal = normalize(uModelInvTransp * vec4(aNorm, 1.0));
    normal = normalize(vec4(aNorm, 1.0));
    // TODO: Correct the normal vectors

    gl_Position = position;
}
