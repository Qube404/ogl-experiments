#include <array>

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
glm::vec3 lightPos(0.0f, 4.0f, 0.0f);

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

void renderQuad() {
    unsigned int quadVAO = 0;
    unsigned int quadVBO;

    if (quadVAO == 0)
    {
        float quadVertices[] = {
            // positions        // texture Coords
            -1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
            -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
             1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
             1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
        };
        // setup plane VAO
        glGenVertexArrays(1, &quadVAO);
        glGenBuffers(1, &quadVBO);
        glBindVertexArray(quadVAO);
        glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    }
    glBindVertexArray(quadVAO);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glBindVertexArray(0);
}

void lightRender(Shader &lightShader, Model &lightObj, glm::mat4 model, glm::mat4 view, glm::mat4 proj) {
        lightShader.use();

        lightShader.setMat4("view", view);
        lightShader.setMat4("proj", proj);

        model = glm::translate(glm::mat4(1.0f), lightPos);
        model = glm::scale(model, glm::vec3(0.25));
        lightShader.setMat4("model", model);

        lightObj.draw(lightShader);
}

void debugRender(Shader &debugShader, unsigned int depthMap, float nearPlane, float farPlane) {
        glViewport(0, 0, IMG_WIDTH, IMG_HEIGHT);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        debugShader.use();
        debugShader.setFloat("nearPlane", nearPlane);
        debugShader.setFloat("farPlane", farPlane);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, depthMap);
        renderQuad();
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

    // Point Shadow Maps
    unsigned int depthMapFBO;
    glGenFramebuffers(1, &depthMapFBO);

    unsigned int depthCubemap;
    glGenTextures(1, &depthCubemap);

    const unsigned int SHADOW_WIDTH = 4096, SHADOW_HEIGHT = 4096;
    glBindTexture(GL_TEXTURE_CUBE_MAP, depthCubemap);

    for (unsigned int i = 0; i != 6; i++) {
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, 
                     GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    }

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
    glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depthCubemap, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    // Objects & Shaders
    Shader objShader("shader/obj_vert.glsl", "shader/obj_frag.glsl");
    Shader depthShader("shader/depth_vert.glsl", "shader/depth_geom.glsl", "shader/depth_frag.glsl");
    Shader debugShader("shader/debug_vert.glsl", "shader/debug_frag.glsl");
    Shader lightShader("shader/light_vert.glsl", "shader/light_frag.glsl");

    Model scene("shapes/point_map_scene.obj");
    Model cube("shapes/cube.obj");

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

        // Depth Pass
        glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
        glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
        glClear(GL_DEPTH_BUFFER_BIT);
        
        float near = 1.0f, far = 25.0f, aspect = (float) SHADOW_WIDTH / SHADOW_HEIGHT;
        
        glm::mat4 shadowProj = glm::perspective(glm::radians(90.0f), aspect, near, far);

        std::array<glm::mat4, 6> shadowTransforms({
            shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3(1.0, 0.0, 0.0), glm::vec3(0.0, -1.0, 0.0)),
            shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3(-1.0, 0.0, 0.0), glm::vec3(0.0, -1.0, 0.0)),
            shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3(0.0, 1.0, 0.0), glm::vec3(0.0, 0.0, 1.0)),
            shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3(0.0, -1.0, 0.0), glm::vec3(0.0, 0.0, -1.0)),
            shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3(0.0, 0.0, 1.0), glm::vec3(0.0, -1.0, 0.0)),
            shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3(0.0, 0.0, -1.0), glm::vec3(0.0, -1.0, 0.0))
        });


        depthShader.use();
        depthShader.setMat4("model", model);

        depthShader.setMat4("shadowMatrices[0]", shadowTransforms[0]);
        depthShader.setMat4("shadowMatrices[1]", shadowTransforms[1]);
        depthShader.setMat4("shadowMatrices[2]", shadowTransforms[2]);
        depthShader.setMat4("shadowMatrices[3]", shadowTransforms[3]);
        depthShader.setMat4("shadowMatrices[4]", shadowTransforms[4]);
        depthShader.setMat4("shadowMatrices[5]", shadowTransforms[5]);

        depthShader.setVec3("lightPos", lightPos);
        depthShader.setFloat("farPlane", far);

        scene.draw(depthShader);

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

        objShader.setFloat("farPlane", far);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, depthCubemap);

        scene.draw(objShader);
        // Object Render

        lightRender(lightShader, cube, model, view, proj);
        //debugRender(debugShader, depthMap, nearPlane, farPlane);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindTexture(GL_TEXTURE_2D, 0);

    objShader.clean();
    depthShader.clean();
    debugShader.clean();

    glfwTerminate();
    return 0;
}
