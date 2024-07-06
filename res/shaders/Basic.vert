#version 410 core

layout(location = 0) in vec3 position;

uniform mat4 u_VP;
uniform mat4 u_M;

void main() {
    gl_Position = u_VP * u_M * vec4(position, 1.0);
}
