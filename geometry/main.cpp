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
bool historyEmpty = true;

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
    Shader normShader("shader/norm_vert.glsl", "shader/norm_geom.glsl", "shader/norm_frag.glsl");
    float objOffset = 10;

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
            } else if (shape == Shape::Sphere) {
                newShape = sphere;
            } else if (shape == Shape::Cylinder) {
                newShape = cylinder;
            } else if (shape == Shape::Plane) {
                newShape = plane;
            } else if (shape == Shape::Cone) {
                newShape = cone;
            }
            newShape.position = glm::vec3(cam.position + (objOffset * cam.front));
            newShape.scale = 1.f;
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

        // Rendering
        glClearColor(bg.r, bg.g, bg.b, bg.a);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glm::mat4 view = cam.getViewMatrix();
        glm::mat4 proj = glm::perspective(glm::radians(cam.fov), (float) img_width / img_height, 0.1f, 100.0f);
        glm::mat4 model = glm::mat4(1.0);

        // Object Draw
        objShader.use();

        objShader.setMat4("view", view);
        objShader.setMat4("proj", proj);

        for (unsigned int i = 0; i != shapes.size(); i++) {
            model = glm::translate(glm::mat4(1.f), shapes[i].position);
            objShader.setMat4("model", model);

            shapes[i].draw(objShader);
        }

        // Normal Draw
        normShader.use();

        normShader.setMat4("view", view);
        normShader.setMat4("proj", proj);

        for (unsigned int i = 0; i != shapes.size(); i++) {
            model = glm::translate(glm::mat4(1.f), shapes[i].position);
            normShader.setMat4("model", model);
            
            shapes[i].draw(normShader);
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
