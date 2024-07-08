#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/ext/matrix_transform.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <cstdlib>
#include <glm/trigonometric.hpp>
#include <iostream>
#include <array>
#include <vector>

#include "Camera.hpp"
#include "Chunk.hpp"
#include "Texture.hpp"
#include "Renderer.hpp"
#include "Shader.hpp"

int main() {
    Vision::Renderer renderer;
    try {
        renderer.Init({1280, 720}, "dev");
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        renderer.Shutdown();
        return EXIT_FAILURE;
    }

    const std::array<Voxel::Vertex, 4> verts = {
        Voxel::Vertex{{-0.5f, -0.5f, -0.5f}, {0.0f, 0.0f}},
        Voxel::Vertex{{-0.5f, -0.5f, 0.5f}, {0.0f, 1.0f}},
        Voxel::Vertex{{0.5f, -0.5f, -0.5f}, {1.0f, 0.0f}},
        Voxel::Vertex{{0.5f, -0.5f, 0.5f}, {1.0f, 1.0f}},
    };

    std::vector<Voxel::Chunk> chunks;
    chunks.reserve(9);
    for (int x = 0; x < 3; x++) {
        for (int z = 0; z < 3; z++) {
            chunks.emplace_back(glm::vec3(x*33.0f, 0.0f, z*33.0f), verts);
        }
    }
    for (size_t i = 0; i < chunks.size(); i++) {
        chunks[i].Build();
    }

    Vision::Shader shader("res/shaders/Basic.vert", "res/shaders/Basic.frag");
    shader.Use();

    Vision::Texture texture("res/textures/texture_atlas.png", true);
    texture.Bind();
    shader.SetInt("u_Texture", 0);

    Vision::Camera cam(renderer, {5.0f, 32.2f, 5.0f}, {0.1f, 500.0f}, 45.0f,
                       5.0f);

    std::cout << "Start Rendering" << std::endl;
    while (!renderer.ShouldClose()) {
        renderer.Clear({1, 1, 1});
        glm::vec2 size = renderer.GetWindowSize();

        cam.Controls(renderer);

        glm::mat4 mvp = cam.getProjMat() * cam.getViewMat();
        shader.Use();
        shader.SetMat4f("u_MVP", mvp);

        for (size_t i = 0; i < chunks.size(); i++) {
            chunks[i].Draw(renderer, shader);
        }

        renderer.Swap();
    }
    std::cout << "End Rendering" << std::endl;

    renderer.Shutdown();
    return 0;
}
