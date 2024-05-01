#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include "stb_image.h"
#include "shader.h"
#include "camera.h"

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

// Mouse Data
float lastX = img_width / 2.0;
float lastY = img_height / 2.0;
bool firstMouse = true;

// Frame Data
float deltaTime = 0;
float lastFrame = 0;

// Flashlight
bool flashlightIsOn = false;

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

void flashlightCallback(GLFWwindow *window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_F && action == GLFW_PRESS) {
        flashlightIsOn = !flashlightIsOn;
    }
}

void processInput(GLFWwindow *window) {
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

    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
        cam.processKeyboard(CameraMovement::DOWN, mode, deltaTime);
    }

    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
        cam.processKeyboard(CameraMovement::UP, mode, deltaTime);
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
    glfwSetKeyCallback(window, flashlightCallback);

    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;

        return -1;
    }

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glViewport(0, 0, img_width, img_height);

    stbi_set_flip_vertically_on_load(true);

    // Object
    Shader objShader("shader/obj_vert.glsl", "shader/obj_frag.glsl");

    float lightVerts[] = {
        -0.5, -0.5, 0.5,
        -0.5, 0.5, 0.5,
        0.5, 0.5, 0.5,
        0.5, -0.5, 0.5,

        -0.5, -0.5, -0.5,
        -0.5, 0.5, -0.5,
        0.5, 0.5, -0.5,
        0.5, -0.5, -0.5,
    };

    unsigned int lightIndices[] = {
        0, 1, 2,
        0, 2, 3,

        4, 5, 6,
        4, 6, 7,

        0, 1, 5,
        0, 5, 4,

        3, 2, 6,
        3, 6, 7,

        0, 4, 7,
        0, 7, 3,

        1, 5, 6,
        1, 6, 2,
    };

    unsigned int lightVBO, lightEBO, lightVAO;

    glGenVertexArrays(1, &lightVAO);
    glBindVertexArray(lightVAO);

    glGenBuffers(1, &lightVBO);
    glBindBuffer(GL_ARRAY_BUFFER, lightVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(lightVerts), lightVerts, GL_STATIC_DRAW);

    glGenBuffers(1, &lightEBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, lightEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(lightIndices), lightIndices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*) 0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    Shader lightShader("shader/light_vert.glsl", "shader/light_frag.glsl");

    glEnable(GL_DEPTH_TEST);
    while(!glfwWindowShouldClose(window)) {
        float currentFrame = (float) glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        processInput(window);
        glClearColor(bg.r, bg.g, bg.b, bg.a);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glm::mat4 view = cam.getViewMatrix();
        glm::mat4 proj = glm::perspective(glm::radians(cam.fov), (float) img_width / img_height, 0.1f, 100.0f);
        glm::mat4 model = glm::mat4(1.0);

        //  Light Render
        lightShader.use();

        model = glm::mat4(1.0);
        model = glm::translate(model, lightPos);
        model = glm::scale(model, lightSize);

        glm::vec3 lightColor(0.8f, 0.5f, 0.6f);
        lightShader.setVec3("lightColor", lightColor);

        lightShader.setMat4("view", view);
        lightShader.setMat4("proj", proj);

        glBindVertexArray(lightVAO);
        for (uint i = 0; i != 4; i++) {
            glm::mat4 model = glm::mat4(1.0f);

            model = glm::translate(model, lightPositions[i]);

            model = glm::scale(model, lightSize);

            lightShader.setMat4("model", model);
            glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindTexture(GL_TEXTURE_2D, 0);

    glDeleteVertexArrays(1, &lightVAO);
    glDeleteBuffers(1, &lightVBO);
    glDeleteBuffers(1, &lightEBO);

    objShader.clean();
    lightShader.clean();

    glfwTerminate();
    return 0;
}
