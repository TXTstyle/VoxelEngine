#version 430 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec2 texCoords;
layout(location = 2) in int voxelData;

uniform mat4 u_MVP;
uniform vec2 u_WorldPos;

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

    int z = voxelData & 15;
    int y = (voxelData >> 4) & 255;
    int x = (voxelData >> 12) & 15;
    int face = (voxelData >> 16) & 7;
    int type = ((voxelData >> 19) & 15) - 1;

    vec3 offset = vec3(x, y, z);
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
        basePos.x *= -1;
        basePos.z++;
    } else if (face == 5) { // zn
        basePos.xyz = basePos.xzy;
    }

    vec2 texPos = vec2(type % 4, type / 4);
    texPos.y = 3 - texPos.y;
    vUV = (texCoords + texPos) * 0.25;

    vNorm = NORMALS[face];
    gl_Position = u_MVP * vec4(basePos + offset + vec3(u_WorldPos.x, 0, u_WorldPos.y) * 16.0, 1.0);
}
