#version 430 core

layout(location = 0) out vec4 color;

in vec2 vUV;
in vec3 vNorm;

uniform sampler2D u_Texture;

void main() {
    color = texture(u_Texture, vUV);
}
