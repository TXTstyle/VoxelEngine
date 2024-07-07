#version 430 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec2 texCoords;
layout(location = 2) in int voxelData;

uniform mat4 u_MVP;
uniform vec3 u_WorldPos;

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

    int z = voxelData & 31;
    int y = (voxelData >> 5) & 31;
    int x = (voxelData >> 10) & 31;
    int face = (voxelData >> 15) & 7;
    int type = (voxelData >> 18) & 15;

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

    vNorm = NORMALS[face];
    vUV = texCoords;
    gl_Position = u_MVP * vec4(basePos + offset + u_WorldPos.xyz, 1.0);
}
