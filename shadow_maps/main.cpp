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

short int shape = Shape::Cube;

// Mouse Data
float lastX = img_width / 2.0;
float lastY = img_height / 2.0;
bool firstMouse = true;

// Key Press Data
bool mouseLeftFirst = true;
bool zKeyFirst = true;
bool xKeyFirst = true;
bool eKeyFirst = true;
bool cKeyFirst = true;
bool historyEmpty = true;

// Frame Data
float deltaTime = 0;
float lastFrame = 0;

// Light Data
glm::vec3 lightPos(-2.0f, 4.0f, -1.0f);

// Debug Data
bool debug = false;

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

    // Weapon Selection
    if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS) {
        shape = Shape::Cube;
    }

    if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS) {
        shape = Shape::Sphere;
    }

    if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS) {
        shape = Shape::Cylinder;
    }

    if (glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS) {
        shape = Shape::Plane;
    }
    
    if (glfwGetKey(window, GLFW_KEY_5) == GLFW_PRESS) {
        shape = Shape::Cone;
    }
}

unsigned int quadVAO = 0;
unsigned int quadVBO;
void renderQuad()
{
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

    // Shadow Maps
    unsigned int depthMapFBO;
    glGenFramebuffers(1, &depthMapFBO);

    const unsigned int SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;

    unsigned int depthMap;
    glGenTextures(1, &depthMap);
    glBindTexture(GL_TEXTURE_2D, depthMap);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

    float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

    glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
    } 
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    // Objects
    float objOffset = 10;

    Shader objShader("shader/obj_vert.glsl", "shader/obj_frag.glsl");
    Shader lightShader("shader/light_vert.glsl", "shader/light_frag.glsl");
    Shader depthShader("shader/depth_vert.glsl", "shader/depth_frag.glsl");
    Shader debugShader("shader/debug_vert.glsl", "shader/debug_frag.glsl");

    Model cube("shapes/cube.obj");
    Model sphere("shapes/sphere.obj");
    Model cylinder("shapes/cylinder.obj");
    Model plane("shapes/plane.obj");
    Model cone("shapes/cone.obj");

    std::vector<Model> shapes;
    std::vector<Model> history;

    // Render Loop
    glEnable(GL_DEPTH_TEST);
    while(!glfwWindowShouldClose(window)) {
        float currentFrame = (float) glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        processInput(window);

        // Placing Shapes
        if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS && mouseLeftFirst == true) {
            Model newShape;
            if (shape == Shape::Cube) {
                newShape = cube;
                newShape.scale = 1.f;
            } else if (shape == Shape::Sphere) {
                newShape = sphere;
                newShape.scale = 1.f;
            } else if (shape == Shape::Cylinder) {
                newShape = cylinder;
                newShape.scale = 1.f;
            } else if (shape == Shape::Plane) {
                newShape = plane;
                newShape.scale = 20.f;
            } else if (shape == Shape::Cone) {
                newShape = cone;
                newShape.scale = 1.f;
            }
            newShape.position = glm::vec3(cam.position + (objOffset * cam.front));
            shapes.push_back(newShape);

            if (historyEmpty == false) {
                history.clear();
            }

            mouseLeftFirst = false;
        }
        mouseLeftFirst = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE;

        // Undo & Redo
        if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS & zKeyFirst == true) {
            if (shapes.size() != 0) {
                history.push_back(shapes[shapes.size() - 1]);
                shapes.pop_back();
            }

            zKeyFirst = false;
        }
        zKeyFirst = glfwGetKey(window, GLFW_KEY_Z) == GLFW_RELEASE;

        if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS & xKeyFirst == true) {
            if (history.size() != 0) {
                shapes.push_back(history[history.size() - 1]);
                history.pop_back();
            }

            xKeyFirst = false;
        }
        xKeyFirst = glfwGetKey(window, GLFW_KEY_X) == GLFW_RELEASE;

        historyEmpty = history.size() == 0;

        // Clear Placed Objects
        if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS & xKeyFirst == true) {
            shapes.clear();
            history.clear();
        }

        // Switch To Debug
        if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS & eKeyFirst == true) {
            debug = !debug;

            eKeyFirst = false;
        }
        eKeyFirst = glfwGetKey(window, GLFW_KEY_E) == GLFW_RELEASE;

        // Setup
        glm::mat4 view = cam.getViewMatrix();
        glm::mat4 proj = glm::perspective(glm::radians(cam.fov), (float) img_width / img_height, 0.1f, 100.0f);
        glm::mat4 model = glm::mat4(1.0);

        // Depth Pass
        glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
        glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
        glClear(GL_DEPTH_BUFFER_BIT);
        
        float near_plane = 1.0f, far_plane = 7.5f;
        
        glm::mat4 lightProj = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane);
        glm::mat4 lightView = glm::lookAt(lightPos, glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        glm::mat4 lightSpaceMatrix = lightProj * lightView;

        depthShader.use();
        depthShader.setMat4("lightSpaceMatrix", lightSpaceMatrix);

        // Floor
        model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -3.0f, 0.0f));
        model = glm::scale(model, glm::vec3(40.0f));
        depthShader.setMat4("model", model);
        plane.draw(depthShader);

        // Objects
        for (unsigned int i = 0; i != shapes.size(); i++) {
            model = glm::translate(glm::mat4(1.0f), shapes[i].position);
            model = glm::scale(model, glm::vec3(shapes[i].scale));
            depthShader.setMat4("model", model);

            shapes[i].draw(depthShader);
        }

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glViewport(0, 0, img_width, img_height);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        if (debug == false) {
            // Render Pass
            glClearColor(bg.r, bg.g, bg.b, bg.a);
            
            // Light Render
            lightShader.use();
            lightShader.setMat4("view", view);
            lightShader.setMat4("proj", proj);

            model = glm::mat4(1.0f);
            model = glm::translate(model, lightPos);

            lightShader.setMat4("model", model);

            cube.draw(lightShader);

            // Object Render
            model = glm::mat4(1.0);

            objShader.use();

            objShader.setMat4("view", view);
            objShader.setMat4("proj", proj);
            objShader.setVec3("lightPos", lightPos);
            objShader.setVec3("viewPos", cam.position);

            objShader.setMat4("lightSpaceMatrix", lightSpaceMatrix);

            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, depthMap);

            model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -3.0f, 0.0f));
            model = glm::scale(model, glm::vec3(40.0f));
            objShader.setMat4("model", model);
            plane.draw(objShader);

            for (unsigned int i = 0; i != shapes.size(); i++) {
                model = glm::translate(glm::mat4(1.f), shapes[i].position);
                model = glm::scale(model, glm::vec3(shapes[i].scale));
                objShader.setMat4("model", model);

                shapes[i].draw(objShader);
            }
        } else {
            // Debug Pass
            debugShader.use();
            debugShader.setFloat("nearPlane", near_plane);
            debugShader.setFloat("farPlane", far_plane);

            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, depthMap);
            renderQuad();
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindTexture(GL_TEXTURE_2D, 0);

    objShader.clean();

    glfwTerminate();
    return 0;
}
