#version 420 core
#extension GL_ARB_explicit_uniform_location : enable

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNorm;

layout(location = 2) uniform mat4 uModel;
layout(location = 3) uniform mat4 uView;
layout(location = 4) uniform mat4 uPersp;

layout(location = 14) uniform mat4 uModelInvTransp;

out vec3 worldPosition;
out vec4 viewPosition;

out vec3 normal;

void main() {
    worldPosition = (uModel * vec4(aPos, 1.0)).xyz;
    viewPosition = uView * vec4(worldPosition, 1.0);
    normal = normalize(uModelInvTransp * vec4(aNorm, 1.0)).xyz;

    gl_Position = uPersp * viewPosition;
}
