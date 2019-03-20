#version 420 core
#extension GL_ARB_explicit_uniform_location : enable

// Interpolated from the vertex shader
in vec3 worldPosition;
in vec4 viewPosition;
in vec3 normal;

layout(location = 3) uniform mat4 uView;
layout(location = 13) uniform vec3 eyePos;

layout(location = 5) uniform vec3 lightPos;
layout(location = 6) uniform vec3 ambientLightColor;
layout(location = 7) uniform vec3 diffuseLightColor;
layout(location = 8) uniform vec3 specularLightColor;

layout(location = 9) uniform vec3 ambientMaterialColor;
layout(location = 10) uniform vec3 diffuseMaterialColor;
layout(location = 11) uniform vec3 specularMaterialColor;
layout(location = 12) uniform float materialShininess;

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
        // Set color according to lighting
        fragColor = vec4(diffuseMaterialColor, 1.0f);

        vec3 fragmentToLight = normalize(lightPos - worldPosition);
        vec3 reflection = normalize(reflect(-fragmentToLight, normal));
        vec3 fragmentToEye = normalize(eyePos - worldPosition);
        vec3 halfway = normalize(fragmentToLight + fragmentToEye);

        float NDotH = max(0.0, dot(normal, halfway));
        float specularWeight = pow(NDotH, materialShininess);
        float diffuseWeight = max(0.0, dot(normal, fragmentToLight));

        vec3 ambient = ambientLightColor * ambientMaterialColor;
        vec3 diffuse = (diffuseLightColor * diffuseMaterialColor) * diffuseWeight;
        vec3 specular = (specularLightColor * specularMaterialColor) * specularWeight;

        fragColor = vec4(ambient + diffuse + specular, 1.0);
    }

    // Overwrite colors at center
    if (abs(worldPosition.x) < 0.3 &&
        abs(worldPosition.y) < 0.3 &&
        abs(worldPosition.z) < 0.3) {
        fragColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);
    }
}
