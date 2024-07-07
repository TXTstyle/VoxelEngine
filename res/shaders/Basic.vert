#version 430 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec2 texCoords;
layout(location = 2) in vec3 offset;

uniform mat4 u_MVP;
//uniform vec3 u_WorldPos;

out vec2 vUV;
out vec3 vNorm;

const vec3 NORMALS[6] = vec3[6](
        vec3(0.0, 1.0, 0.0), //  +y
        vec3(0.0, -1.0, 0.0), // -y
        vec3(1.0, 0.0, 0.0), //  +x
        vec3(-1.0, 0.0, 0.0), // -x
        vec3(0.0, 0.0, 1.0), //  +z
        vec3(0.0, 0.0, -1.0) // -z
    );

void main() {
    int face = int(offset.x);
    vec3 basePos = position;

    if (face == 0) { // yp
        basePos.y++;
    } else if (face == 1) { // yn
        basePos.x *= -1;
    } else if (face == 2) { // xp
        basePos.xy = -basePos.yx;
    } else if (face == 3) { // xn
        basePos.xy = basePos.yx;
    } else if (face == 4) { // zp
        basePos.xyz = basePos.xzy;
    } else if (face == 5) { // zn
        basePos.xyz = basePos.xzy;
        basePos.x *= -1;
        basePos.z++;
    }

    vNorm = NORMALS[0];
    vUV = texCoords;
    gl_Position = u_MVP * vec4(basePos, 1.0);
}
