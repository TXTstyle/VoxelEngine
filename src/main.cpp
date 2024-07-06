#include <cstdint>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/ext/matrix_transform.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <cstdlib>
#include <glm/trigonometric.hpp>
#include <iostream>
#include <vector>

#include "Camera.hpp"
#include "IndexBuffer.hpp"
#include "Renderer.hpp"
#include "Shader.hpp"
#include "VertexArray.hpp"
#include "VertexBuffer.hpp"
#include "VertexBufferLayout.hpp"

struct Vertex {
    glm::vec3 pos;
};

std::vector<uint32_t> GenIndices(uint32_t maxCount) {
    std::vector<uint32_t> indices(maxCount);
    uint32_t indOffset = 0;
    for (int i = 0; i < maxCount; i += 36) {
        indices[i + 0] = 0 + indOffset;
        indices[i + 1] = 3 + indOffset;
        indices[i + 2] = 2 + indOffset;

        indices[i + 3] = 0 + indOffset;
        indices[i + 4] = 2 + indOffset;
        indices[i + 5] = 1 + indOffset;

        indices[i + 6] = 0 + indOffset;
        indices[i + 7] = 1 + indOffset;
        indices[i + 8] = 5 + indOffset;

        indices[i + 9] = 0 + indOffset;
        indices[i + 10] = 5 + indOffset;
        indices[i + 11] = 4 + indOffset;

        indices[i + 12] = 6 + indOffset;
        indices[i + 13] = 1 + indOffset;
        indices[i + 14] = 2 + indOffset;

        indices[i + 15] = 6 + indOffset;
        indices[i + 16] = 5 + indOffset;
        indices[i + 17] = 1 + indOffset;

        indices[i + 18] = 2 + indOffset;
        indices[i + 19] = 7 + indOffset;
        indices[i + 20] = 6 + indOffset;

        indices[i + 21] = 2 + indOffset;
        indices[i + 22] = 3 + indOffset;
        indices[i + 23] = 7 + indOffset;

        indices[i + 24] = 0 + indOffset;
        indices[i + 25] = 4 + indOffset;
        indices[i + 26] = 3 + indOffset;

        indices[i + 27] = 3 + indOffset;
        indices[i + 28] = 4 + indOffset;
        indices[i + 29] = 7 + indOffset;

        indices[i + 30] = 6 + indOffset;
        indices[i + 31] = 4 + indOffset;
        indices[i + 32] = 5 + indOffset;

        indices[i + 33] = 6 + indOffset;
        indices[i + 34] = 7 + indOffset;
        indices[i + 35] = 4 + indOffset;

        indOffset += 8;
    }
    return indices;
}

int main() {
    Vision::Renderer renderer;
    try {
        renderer.Init({1280, 720}, "dev");
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        renderer.Shutdown();
        return EXIT_FAILURE;
    }
    float pos[] = {
        -0.5f, -0.5f, -0.5f, // 1
        0.5f,  -0.5f, -0.5f, // 2
        0.5f,  -0.5f, 0.5f,  // 3
        -0.5f, -0.5f, 0.5f,  // 4

        -0.5f, 0.5f,  -0.5f, // 5
        0.5f,  0.5f,  -0.5f, // 6
        0.5f,  0.5f,  0.5f,  // 7
        -0.5f, 0.5f,  0.5f,  // 8
    };

    uint32_t indices[] = {
        0, 3, 2, //
        0, 2, 1, //
        0, 1, 5, //
        5, 4, 0, //
        6, 1, 2, //
        6, 5, 1, //
        2, 7, 6, //
        2, 3, 7, //
        0, 4, 3, //
        3, 4, 7, //
        6, 7, 4, //
        6, 4, 5, //

    };

    Vision::VertexArray va;
    Vision::VertexBuffer vb(pos, sizeof(pos));

    Vision::VertexBufferLayout layout;
    layout.Push<float>(3);

    va.AddBuffer(vb, layout);
    Vision::IndexBuffer ib(indices, 36);

    Vision::Shader shader("res/shaders/Basic.vert", "res/shaders/Basic.frag");
    shader.Use();

    Vision::Camera cam(renderer, {0.0f, 0.0f, 10.0f}, {0.1f, 100.0f}, 60.0f, 3.0f);

    while (!renderer.ShouldClose()) {
        renderer.Clear({1, 1, 1});
        glm::vec2 size = renderer.GetWindowSize();

        cam.Controls(renderer);

        glm::mat4 model(1.0f);
        glm::mat4 vp = cam.getProjMat() * cam.getViewMat();
        shader.Use();
        shader.SetMat4f("u_VP", vp);
        shader.SetMat4f("u_M", model);

        renderer.Draw(va, ib, shader);

        model = glm::translate(model, {2.0f, 0.0f, 0.0f});
        shader.SetMat4f("u_M", model);

        renderer.Draw(va, ib, shader);

        renderer.Swap();
    }

    renderer.Shutdown();
    return 0;
}
