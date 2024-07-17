#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include "stb_image.h"
#include "shader.h"
#include "camera.h"
#include "model.h"

const unsigned int IMG_WIDTH = 1920; const unsigned int IMG_HEIGHT = 1080;
Camera cam(glm::vec3(0, 0, 0), glm::vec3(0, 1, 0), -90, 0, 4, 8, 16, 0.065f, 90, 120);

glm::vec4 bg = glm::vec4(0.156863, 0.160784, 0.211765, 1);

// Mouse Data
float lastX = IMG_WIDTH / 2.0;
float lastY = IMG_HEIGHT / 2.0;
bool firstMouse = true;

// Frame Data
float deltaTime = 0;
float lastFrame = 0;

// Light Data
glm::vec3 lightPos(-10.0f, 12.0f, 2.0f);

void framebufferSizeCallback(GLFWwindow *window, int width, int height) {
    glViewport(0, 0, width, height);
}

void mouseCallback(GLFWwindow *window, double posX, double posY) {
    if (firstMouse) {
        lastX = (float) posX;
        lastY = (float) posY;

        firstMouse = false;
    }

    float offsetX = (float) (posX - lastX);
    float offsetY = (float) (lastY - posY);

    lastX = (float) posX;
    lastY = (float) posY;

    cam.processMouse(offsetX, offsetY);
}

void scrollCallback(GLFWwindow *window, double offsetX, double offsetY) {
    cam.processMouseScroll((float) offsetY);
}

void processInput(GLFWwindow *window) {
    // Movement
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }

    MovementType mode = MovementType::JOGGING;
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
        mode = MovementType::RUNNING;
    }

    if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) {
        mode = MovementType::WALKING;
    }

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        cam.processKeyboard(CameraMovement::FORWARD, mode, deltaTime);
    }

    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        cam.processKeyboard(CameraMovement::BACKWARD, mode, deltaTime);
    }

    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        cam.processKeyboard(CameraMovement::RIGHT, mode, deltaTime);
    }

    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        cam.processKeyboard(CameraMovement::LEFT, mode, deltaTime);
    }

    if (glfwGetKey(window, GLFW_KEY_LEFT_ALT) == GLFW_PRESS) {
        cam.processKeyboard(CameraMovement::DOWN, mode, deltaTime);
    }

    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
        cam.processKeyboard(CameraMovement::UP, mode, deltaTime);
    }
}

int main() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(IMG_WIDTH, IMG_HEIGHT, "QubeGL", NULL, NULL);

    if (window == NULL) {
        std::cout << "Failed to create GLFW window" << std::endl;

        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);
    glfwSetCursorPosCallback(window, mouseCallback);
    glfwSetScrollCallback(window, scrollCallback);

    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;

        return -1;
    }

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glViewport(0, 0, IMG_WIDTH, IMG_HEIGHT);

    stbi_set_flip_vertically_on_load(true);

    // Objects & Shaders
    Shader objShader("shader/obj_vert.glsl", "shader/obj_frag.glsl");
    Shader lightShader("shader/light_vert.glsl", "shader/light_frag.glsl");

    Model scene("shapes/test_scene.obj");

    // Render Loop
    glEnable(GL_DEPTH_TEST);
    while(!glfwWindowShouldClose(window)) {
        float currentFrame = (float) glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        processInput(window);
        glClearColor(bg.r, bg.g, bg.b, bg.a);

        // Setup
        glm::mat4 view = cam.getViewMatrix();
        glm::mat4 proj = glm::perspective(glm::radians(cam.fov), (float) IMG_WIDTH / IMG_HEIGHT, 0.1f, 100.0f);
        glm::mat4 model = glm::mat4(1.0);

        // Object Render
        glViewport(0, 0, IMG_WIDTH, IMG_HEIGHT);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glCullFace(GL_BACK);

        objShader.use();

        objShader.setMat4("view", view);
        objShader.setMat4("proj", proj);
        objShader.setMat4("model", model);

        objShader.setVec3("lightPos", lightPos);
        objShader.setVec3("viewPos", cam.position);

        scene.draw(objShader);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindTexture(GL_TEXTURE_2D, 0);

    objShader.clean();
    lightShader.clean();

    glfwTerminate();
    return 0;
}
