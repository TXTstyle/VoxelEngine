#include "Renderer.hpp"
#include "Shader.hpp"
#include <GLFW/glfw3.h>
#include <glm/ext/matrix_transform.hpp>
#include <stdexcept>
#include <iostream>

void GLAPIENTRY Vision::MessageCallback(GLenum source, GLenum type, GLuint id,
                                        GLenum severity, GLsizei length,
                                        const GLchar* message,
                                        const void* userParam) {
    if (severity > 0x826b) {
        printf("Message from OpenGL:\nSource: 0x%x\nType: 0x%x\n"
               "Id: 0x%x\nSeverity: 0x%x\n",
               source, type, id, severity);
        printf("%s\n", message);
    }
}

using namespace Vision;

Renderer::Renderer() {}

void Renderer::Init(const glm::vec2 windowSize, const std::string windowName) {
    this->windowSize = windowSize;

    // Initialize the library
    if (!glfwInit())
        throw std::runtime_error("Failed to initialize GLFW");

    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Create a windowed mode window and its OpenGL context
    window = glfwCreateWindow(windowSize.x, windowSize.y, windowName.c_str(),
                              NULL, NULL);
    if (!window) {
        glfwTerminate();
        throw std::runtime_error("Failed to create GLFW window");
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);
    glfwSetWindowUserPointer(window, this);
    glfwSetFramebufferSizeCallback(window, ResizeCallback);
    glfwSetCursorPosCallback(window, CursorCallback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    glfwSwapInterval(1);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
        throw std::runtime_error("Failed to initialize GLAD");

    std::cout << glGetString(GL_VERSION) << std::endl;

    glEnable(GL_DEBUG_OUTPUT);
    glDebugMessageCallback(MessageCallback, NULL);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glEnable(GL_DEPTH_TEST);
    // glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    glViewport(0, 0, windowSize.x, windowSize.y);
}

void Renderer::Shutdown() { glfwTerminate(); }

void Renderer::Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) {
    va.Bind();
    ib.Bind();
    shader.Use();
    glDrawElements(GL_TRIANGLES, ib.GetCount(), GL_UNSIGNED_INT, nullptr);
}

void GenOriVerts(const glm::vec3 rot, glm::vec2 sizeOffset, glm::vec3 p_pos,
                 glm::vec4* pos) {
    glm::mat4 model(1.0f);
    model = glm::rotate(model, glm::radians(rot.x), {1.0f, 0, 0});
    model = glm::rotate(model, glm::radians(rot.y), {0, 1.0f, 0});
    model = glm::rotate(model, glm::radians(rot.z), {0, 0, 1.0f});

    pos[0] = model * glm::vec4(-sizeOffset.x, -sizeOffset.y, 0.0f, 1.0f);
    pos[1] = model * glm::vec4(sizeOffset.x, -sizeOffset.y, 0.0f, 1.0f);
    pos[2] = model * glm::vec4(sizeOffset.x, sizeOffset.y, 0.0f, 1.0f);
    pos[3] = model * glm::vec4(-sizeOffset.x, sizeOffset.y, 0.0f, 1.0f);

    model = glm::translate(glm::mat4(1.0f), p_pos);
    pos[0] = model * pos[0];
    pos[1] = model * pos[1];
    pos[2] = model * pos[2];
    pos[3] = model * pos[3];
}

void Renderer::Clear(const glm::vec3 color) {
    glClearColor(color.x, color.y, color.z, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

bool Renderer::ShouldClose() { return (bool)glfwWindowShouldClose(window); }

void Renderer::Swap() {
    glfwSwapBuffers(window);
    glfwPollEvents();
}

void frameSizeCallback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

void Renderer::SetWindowTitle(std::string title) {
    glfwSetWindowTitle(window, title.c_str());
}

void Renderer::ResizeCallback(GLFWwindow* window, int width, int height) {
    auto app = reinterpret_cast<Renderer*>(glfwGetWindowUserPointer(window));
    app->windowSize.x = width;
    app->windowSize.y = height;
    glViewport(0, 0, width, height);
}

void Renderer::CursorCallback(GLFWwindow* window, double xpos, double ypos) {
    auto app = reinterpret_cast<Renderer*>(glfwGetWindowUserPointer(window));
    app->mousePos.x = xpos;
    app->mousePos.y = ypos;
}
