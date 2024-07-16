#ifndef CAMERA_H
#define CAMERA_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>
#include <algorithm>

enum CameraMovement {
    UP,
    DOWN,
    FORWARD,
    BACKWARD,
    RIGHT,
    LEFT,
};

enum MovementType {
    WALKING,
    JOGGING,
    RUNNING
};

const float DEF_YAW = -90;
const float DEF_PITCH = 0;
const float DEF_WALK_SPEED = 2.5;
const float DEF_JOG_SPEED = 5;
const float DEF_RUN_SPEED = 10;
const float DEF_SENSITIVITY = 0.1f;
const float DEF_FOV = 90;
const float DEF_MAX_FOV = 120;

class Camera {
public:
    glm::vec3 position;
    glm::vec3 front;
    glm::vec3 up;
    glm::vec3 right;
    glm::vec3 worldUp;
    glm::vec3 worldFront;

    float yaw;
    float pitch;

    float walkSpeed;
    float jogSpeed;
    float runSpeed;

    float mouseSensitivity;
    float fov;
    float maxFov;

    Camera(
        glm::vec3 position,
        glm::vec3 up,
        float yaw,
        float pitch,
        float walkSpeed,
        float jogSpeed,
        float runSpeed,
        float mouseSensitivity,
        float fov,
        float maxFov
    );

    glm::mat4 getViewMatrix();

    void processKeyboard(CameraMovement dir, MovementType move, float deltaTime);
    void processMouse(float offsetX, float offsetY);
    void processMouseScroll(float yoffset);

private:
    void updateCamera();
};

#endif
