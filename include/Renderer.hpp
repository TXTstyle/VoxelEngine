#pragma once
#include "Shader.hpp"
#include "VertexArray.hpp"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <string>

namespace Vision {

void GLAPIENTRY MessageCallback(GLenum source, GLenum type, GLuint id,
                                GLenum severity, GLsizei length,
                                const GLchar* message, const void* userParam);

class Renderer {
  public:
    Renderer();

    void Init(const glm::vec2 windowSize, const std::string windowName);
    void Shutdown();

    void Draw(const VertexArray& va, const Shader& shader, uint32_t instanceCount);

    void Clear(const glm::vec3 color);
    bool ShouldClose();
    void Swap();
    void SetWindowTitle(std::string title);

    inline glm::vec2& GetWindowSize() { return windowSize; }
    inline GLFWwindow* GetWindow() { return window; }
    inline glm::vec2& GetMousePos() { return mousePos; }

  private:
    GLFWwindow* window;
    glm::vec2 windowSize;

    glm::vec2 mousePos;

    static void ResizeCallback(GLFWwindow* window, int width, int height);
    static void CursorCallback(GLFWwindow* window, double xpos, double ypos);
};
} // namespace Vision
