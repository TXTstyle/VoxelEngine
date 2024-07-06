#pragma once
#include <glm/glm.hpp>

#include "Renderer.hpp"

namespace Vision {
class Camera {
  private:
    const glm::vec2 camClip;
    float camSpeed;
    float camFov;
    float yaw = -90.0f;
    float pitch = 0.0f;
    float lastX = 0.0f, lastY = 0.0f;
    bool firstMouse = true;
    float sensitivity = 0.075f;


    glm::vec3 camPos = glm::vec3(0.0f, 0.0f, 1.0f);
    glm::vec3 camFront = glm::vec3(0.0f, 0.0f, 1.0f);
    glm::vec3 camUp = glm::vec3(0.0f, 1.0f, 0.0f);

    float deltaTime;
    float lastFrame;

    glm::mat4 projMat;
    glm::mat4 viewMat;

    bool cursorOn = false;
    bool cursorToggle = false;

  public:
    Camera(Renderer& renderer, const glm::vec3 startPos, const glm::vec2 clip, float fov, float camSpeed);
    ~Camera();

    glm::mat4& getProjMat();
    glm::mat4& getViewMat();

    void Controls(Renderer& renderer);
};
} // namespace Vision
