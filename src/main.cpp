#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/ext/matrix_transform.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <cstdlib>
#include <iostream>

#include "Camera.hpp"
#include "ChunkManager.hpp"
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

    int renderDistance = 3;
    Voxel::Manager chunks(renderDistance, 12345u);

    Vision::Shader shader("res/shaders/Basic.vert", "res/shaders/Basic.frag");
    shader.Use();

    Vision::Texture texture("res/textures/texture_atlas.png", true);
    texture.Bind();
    shader.SetInt("u_Texture", 0);

    Vision::Camera cam(renderer, {0.0f, 100.0f, 0.0f}, {0.1f, 200.0f}, 45.0f,
                       5.0f);

    std::cout << "Start Rendering" << std::endl;
    while (!renderer.ShouldClose()) {
        renderer.Clear({0.61, 0.8, 1});
        cam.Controls(renderer);

        glm::mat4 mvp = cam.getProjMat() * cam.getViewMat();
        shader.Use();
        shader.SetMat4f("u_MVP", mvp);

        chunks.Load(cam.GetChunkPos());
        chunks.Build();
        chunks.Draw(renderer, shader);

        renderer.Swap();
    }
    std::cout << "End Rendering" << std::endl;

    renderer.Shutdown();
    return 0;
}
