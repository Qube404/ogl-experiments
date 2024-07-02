#include "camera.h"

const float MAX_PITCH = 89;
const float MIN_PITCH = -89;

Camera::Camera(
    glm::vec3 position, 
    glm::vec3 up, 
    float yaw = DEF_YAW, 
    float pitch = DEF_PITCH,
    float walkSpeed = DEF_WALK_SPEED,
    float jogSpeed = DEF_JOG_SPEED,
    float runSpeed = DEF_RUN_SPEED,
    float mouseSensitivity = DEF_SENSITIVITY,
    float fov = DEF_FOV,
    float maxFov = DEF_MAX_FOV
):
    position(position),
    front(glm::vec3(0, 0, -1)),
    up(0),
    right(0),
    worldUp(up),
    worldFront(glm::vec3(0, 0, -1)),
    yaw(yaw),
    pitch(pitch),
    walkSpeed(walkSpeed),
    jogSpeed(jogSpeed),
    runSpeed(runSpeed),
    mouseSensitivity(mouseSensitivity),
    fov(fov),
    maxFov(maxFov)
{
    updateCamera();
}

glm::mat4 Camera::getViewMatrix() {
    return glm::lookAt(position, position + front, up);
}

void Camera::processKeyboard(CameraMovement dir, MovementType move, float deltaTime) {
    float velocity = 0.0f;

    if (move == MovementType::WALKING) velocity = walkSpeed * deltaTime;
    if (move == MovementType::JOGGING) velocity = jogSpeed * deltaTime;
    if (move == MovementType::RUNNING) velocity = runSpeed * deltaTime;

    if (dir == CameraMovement::UP) {
        position.y += velocity;
    }

    if (dir == CameraMovement::DOWN) {
        position.y -= velocity;
    }

    if (dir == CameraMovement::FORWARD) {
        position = position + worldFront * velocity;
    }

    if (dir == CameraMovement::BACKWARD) {
        position = position - worldFront * velocity;
    }

    if (dir == CameraMovement::RIGHT) {
        position = position + right * velocity;
    }

    if (dir == CameraMovement::LEFT) {
        position = position - right * velocity;
    }
}

void Camera::processMouse(float offsetX, float offsetY) {
    offsetX *= mouseSensitivity;
    offsetY *= mouseSensitivity;

    yaw += offsetX;
    pitch += offsetY;

    if (pitch > MAX_PITCH) {
        pitch = MAX_PITCH;
    }

    if (pitch < MIN_PITCH) {
        pitch = MIN_PITCH;
    }

    updateCamera();
}

void Camera::processMouseScroll(float yoffset) {
    fov = std::clamp(fov - yoffset, 1.0f, maxFov);
}

void Camera::updateCamera() {
    glm::vec3 newFront(
        cos(glm::radians(yaw)) * cos(glm::radians(pitch)),
        sin(glm::radians(pitch)),
        sin(glm::radians(yaw)) * cos(glm::radians(pitch))
    );

    front = glm::normalize(newFront);
    right = glm::normalize(glm::cross(front, worldUp));
    up = glm::normalize(glm::cross(right, front));

    worldFront = glm::normalize(glm::cross(worldUp, right));
}
