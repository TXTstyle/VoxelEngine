#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/ext/matrix_transform.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <cstdlib>
#include <glm/trigonometric.hpp>
#include <iostream>
#include <array>

#include "Camera.hpp"
#include "Texture.hpp"
#include "Renderer.hpp"
#include "Shader.hpp"
#include "VertexArray.hpp"
#include "VertexBuffer.hpp"
#include "VertexBufferLayout.hpp"


struct Vertex {
    glm::vec3 pos;
    glm::vec2 uv;
};

int main() {
    Vision::Renderer renderer;
    try {
        renderer.Init({1280, 720}, "dev");
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        renderer.Shutdown();
        return EXIT_FAILURE;
    }

    std::array<Vertex, 4> verts;
    verts[0] = Vertex{{-0.5f, -0.5f, -0.5f}, {0.0f, 0.0f}};
    verts[1] = Vertex{{-0.5f, -0.5f, 0.5f}, {0.0f, 1.0f}};
    verts[2] = Vertex{{0.5f, -0.5f, -0.5f}, {1.0f, 0.0f}};
    verts[3] = Vertex{{0.5f, -0.5f, 0.5f}, {1.0f, 1.0f}};

    Vision::VertexArray va;
    Vision::VertexBuffer vb(verts.data(), verts.size() * sizeof(Vertex));

    Vision::VertexBufferLayout layout;
    layout.Push<float>(3);
    layout.Push<float>(2);
    va.AddBuffer(vb, layout);

    std::array<glm::vec3, 6> translations;
    translations[0] = {0.0f, 0.0f, 0.0f};
    translations[1] = {1.0f, 0.0f, 0.0f};
    translations[2] = {2.0f, 0.0f, 0.0f};
    translations[3] = {3.0f, 0.0f, 0.0f};
    translations[4] = {4.0f, 0.0f, 0.0f};
    translations[5] = {5.0f, 0.0f, 0.0f};

    Vision::VertexBuffer ivb(translations.data(),
                             translations.size() * sizeof(glm::vec3));
    glEnableVertexAttribArray(2);
    ivb.Bind();
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3),
                          (void*)0);
    ivb.Unbind();
    glVertexAttribDivisor(2, 1);

    Vision::Shader shader("res/shaders/Basic.vert", "res/shaders/Basic.frag");
    shader.Use();

    Vision::Texture texture("res/textures/Grass_texture.png", true);
    texture.Bind();
    shader.SetInt("u_Texture", 0);

    Vision::Camera cam(renderer, {0.0f, 1.0f, 5.0f}, {0.1f, 100.0f}, 45.0f,
                       3.0f);

    while (!renderer.ShouldClose()) {
        renderer.Clear({1, 1, 1});
        glm::vec2 size = renderer.GetWindowSize();

        cam.Controls(renderer);

        glm::mat4 model(1.0f);
        glm::mat4 mvp = cam.getProjMat() * cam.getViewMat() * model;
        shader.Use();
        shader.SetMat4f("u_MVP", mvp);

        renderer.Draw(va, shader, 6);

        renderer.Swap();
    }

    renderer.Shutdown();
    return 0;
}
