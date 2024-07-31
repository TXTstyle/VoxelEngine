#include "Camera.hpp"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Renderer.hpp"

using namespace Vision;

Camera::Camera(Renderer& renderer, const glm::vec3 startPos,
               const glm::vec2 clip, float fov, float camSpeed)
    : camClip(clip), camSpeed(camSpeed), camFov(fov), camPos(startPos) {
    projMat = glm::perspective(glm::radians(camFov),
                               renderer.GetWindowSize().x /
                                   renderer.GetWindowSize().y,
                               camClip.x, camClip.y);
    viewMat = glm::lookAt(camPos, glm::vec3(0.0f), camUp);
}

Camera::~Camera() {}

void Camera::Controls(Renderer& renderer) {
    float mouseX = 0;
    float mouseY = 0;

    if (firstMouse) // initially set to true
    {
        lastX = mouseX;
        lastY = mouseX;
        firstMouse = false;
    }

    mouseX = lastX;
    mouseY = lastY;
    if (!cursorOn) {
        mouseX = renderer.GetMousePos().x;
        mouseY = renderer.GetMousePos().y;
    }

    float xoffset = mouseX - lastX;
    float yoffset =
        lastY - mouseY; // reversed since y-coordinates range from bottom to top
    lastX = mouseX;
    lastY = mouseY;

    xoffset *= sensitivity;
    yoffset *= sensitivity;

    yaw += xoffset;
    pitch += yoffset;

    if (pitch > 89.0f)
        pitch = 89.0f;
    if (pitch < -89.0f)
        pitch = -89.0f;

    glm::vec3 direction;
    direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    direction.y = sin(glm::radians(pitch));
    direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    camFront = glm::normalize(direction);

    if (glfwGetKey(renderer.GetWindow(), GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(renderer.GetWindow(), GLFW_TRUE);
        return;
    }

    if (glfwGetKey(renderer.GetWindow(), GLFW_KEY_LEFT_ALT) == GLFW_PRESS) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    } else {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }

    bool cursorToggleCurrent =
        glfwGetKey(renderer.GetWindow(), GLFW_KEY_I) == GLFW_PRESS;
    if (cursorToggleCurrent && !cursorToggle) {
        cursorOn = !cursorOn;
        int cursorState = cursorOn ? GLFW_CURSOR_NORMAL : GLFW_CURSOR_DISABLED;
        glfwSetInputMode(renderer.GetWindow(), GLFW_CURSOR, cursorState);
        glfwSetCursorPos(renderer.GetWindow(), 0, 0);
        mouseX = 0;
        mouseY = 0;
    }
    cursorToggle = cursorToggleCurrent;

    float currentFrame = glfwGetTime();
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;

    float speed = deltaTime * camSpeed;
    if (glfwGetKey(renderer.GetWindow(), GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
        speed *= 10.0f;
    } else if (glfwGetKey(renderer.GetWindow(), GLFW_KEY_LEFT_CONTROL) ==
               GLFW_PRESS) {
        speed *= 0.5f;
    } else {
        speed = deltaTime * camSpeed;
    }

    if (glfwGetKey(renderer.GetWindow(), GLFW_KEY_W) == GLFW_PRESS) {
        camPos += speed * camFront;
    }
    if (glfwGetKey(renderer.GetWindow(), GLFW_KEY_S) == GLFW_PRESS) {
        camPos -= speed * camFront;
    }
    if (glfwGetKey(renderer.GetWindow(), GLFW_KEY_A) == GLFW_PRESS) {
        camPos -= glm::normalize(glm::cross(camFront, camUp)) * speed;
    }
    if (glfwGetKey(renderer.GetWindow(), GLFW_KEY_D) == GLFW_PRESS) {
        camPos += glm::normalize(glm::cross(camFront, camUp)) * speed;
    }

    viewMat = glm::lookAt(camPos, camPos + camFront, camUp);
    projMat = glm::perspective(glm::radians(camFov),
                               renderer.GetWindowSize().x /
                                   renderer.GetWindowSize().y,
                               camClip.x, camClip.y);
}

glm::ivec2 Camera::GetChunkPos() {
    glm::ivec2 pos = {camPos.x, camPos.z}; 
    return pos / 16;
}

glm::mat4& Camera::getProjMat() { return projMat; }

glm::mat4& Camera::getViewMat() { return viewMat; }
