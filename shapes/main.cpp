#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include "stb_image.h"
#include "shader.h"
#include "camera.h"
#include "model.h"

const unsigned int img_width = 1920; const unsigned int img_height = 1080;
Camera cam(glm::vec3(0, 0, 0), glm::vec3(0, 1, 0), -90, 0, 4, 8, 16, 0.065f, 90, 120);

// Colors
glm::vec4 editorBackground = glm::vec4(0.156863, 0.160784, 0.211765, 1);
glm::vec4 black = glm::vec4(0.0, 0.0, 0.0, 1.0);

// Background Color
glm::vec4 bg = editorBackground;

// Player Data
float playerHeight = 7;
float playerSize = 1;

short int shapes = 1;

// Mouse Data
float lastX = img_width / 2.0;
float lastY = img_height / 2.0;
bool firstMouse = true;
bool mouseLeftPressed = false;

// Frame Data
float deltaTime = 0;
float lastFrame = 0;

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

    if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) {
        cam.processKeyboard(CameraMovement::DOWN, mode, deltaTime);
    }

    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
        cam.processKeyboard(CameraMovement::UP, mode, deltaTime);
    }

    // Weapon Selection
    if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS) {
        shapes = 1;
    }

    if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS) {
        shapes = 2;
    }

    if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS) {
        shapes = 3;
    }

    if (glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS) {
        shapes = 4;
    }
}

int main() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(img_width, img_height, "QubeGL", NULL, NULL);

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
    glViewport(0, 0, img_width, img_height);

    stbi_set_flip_vertically_on_load(true);

    // Objects
    Shader objShader("shader/obj_vert.glsl", "shader/obj_frag.glsl");
    float objOffset = 10;

    Model cube("shapes/cube.obj");
    Model sphere("shapes/sphere.obj");
    Model cylinder("shapes/cylinder.obj");
    Model plane("shapes/plane.obj");
    Model cone("shapes/cone.obj");

    // WORKING ON THIS
    // Currently working on spawning different shapes by pressing different
    // numbers and trying to use integers for boolean logic instead of making
    // a bunch of separate bool variables.
    std::vector<Shape> shapes;
    std::vector<glm::vec3> positions;

    // Render Setup
    bool mouseLeftFirst = true;

    // Render Loop
    glEnable(GL_DEPTH_TEST);
    while(!glfwWindowShouldClose(window)) {
        float currentFrame = (float) glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        processInput(window);

        if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS && mouseLeftFirst == true) {
            glm::vec3 position(cam.position + (objOffset * cam.front));
            positions.push_back(position);

            mouseLeftFirst = false;
        }

        mouseLeftFirst = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE;

        glClearColor(bg.r, bg.g, bg.b, bg.a);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glm::mat4 view = cam.getViewMatrix();
        glm::mat4 proj = glm::perspective(glm::radians(cam.fov), (float) img_width / img_height, 0.1f, 100.0f);
        glm::mat4 model = glm::mat4(1.0);

        // obj Render
        objShader.use();

        objShader.setMat4("view", view);
        objShader.setMat4("proj", proj);

        for (unsigned int i = 0; i != positions.size(); i++) {
            model = glm::translate(glm::mat4(1.0), positions[i]);

            objShader.setMat4("model", model);

            cube.draw(objShader);
        }

        model = glm::mat4(1.0);
        model = glm::translate(model, cam.position + objOffset);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindTexture(GL_TEXTURE_2D, 0);

    objShader.clean();

    glfwTerminate();
    return 0;
}
