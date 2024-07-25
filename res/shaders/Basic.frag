#version 430 core

layout(location = 0) out vec4 color;

in vec2 vUV;
in vec3 vNorm;

uniform sampler2D u_Texture;

void main() {
    vec3 light = vec3(1.);
    vec3 norm = normalize(vNorm);
    if (norm.x > 0.9 || norm.z > 0.9 || norm.y < -0.9) {
        light *= 0.8;
    }

    vec3 finalColor = texture(u_Texture, vUV).rgb * light;
    color = vec4(finalColor, 1.0);
}
