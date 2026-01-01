#include "Application.h"
#include "ShaderProgram.h"
#include "SkyBox.h"

int currentSceneIndex = 0;
static Application* s_instance = nullptr;

float lastX = 400, lastY = 300;
bool firstMouse = true;
float deltaTime = 0.0f;
float lastFrame = 0.0f;
bool keys[1024];
bool mouseDisabled = false;
bool test = false;

Application::Application() : window(nullptr), scene(nullptr), firstMouse(true) {
    s_instance = this;
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    window = glfwCreateWindow(800, 600, "ZPG PROJEKT", nullptr, nullptr);
    glfwMakeContextCurrent(window);
    glewExperimental = GL_TRUE;
    glewInit();
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_STENCIL_TEST);
    glClearStencil(0);
    glfwSetKeyCallback(window, keyCallback);
    glfwSetMouseButtonCallback(window, keyMouseCallback);
    glfwSetCursorPosCallback(window, mouseMoveCallback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
}

void Application::processInput(GLFWwindow* window) {
    if (keys[GLFW_KEY_W])
        s_instance->camera.processKeyboard('W', deltaTime);
    if (keys[GLFW_KEY_S])
        s_instance->camera.processKeyboard('S', deltaTime);
    if (keys[GLFW_KEY_A])
        s_instance->camera.processKeyboard('A', deltaTime);
    if (keys[GLFW_KEY_D])
        s_instance->camera.processKeyboard('D', deltaTime);
}

void Application::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (key >= 0 && key < 1024) {
        if (action == GLFW_PRESS)
            keys[key] = true;
        else if (action == GLFW_RELEASE)
            keys[key] = false;
    }

    if (action == GLFW_PRESS) {
        switch (key) {
            case GLFW_KEY_1: currentSceneIndex = 0; break;
            case GLFW_KEY_2: currentSceneIndex = 1; break;
            case GLFW_KEY_3: currentSceneIndex = 2; break;
            case GLFW_KEY_4: currentSceneIndex = 3; test = false; break;
            case GLFW_KEY_5: currentSceneIndex = 4; break;
            case GLFW_KEY_6: currentSceneIndex = 5; break;
            case GLFW_KEY_R:
                if (s_instance->scene->getSelectedID() > 0) {
                    s_instance->scene->remove(s_instance->scene->getSelectedID());
                }
                break;
        }
        //std::cout << "Scene: " << currentSceneIndex << std::endl;
    }
}

void Application::keyMouseCallback(GLFWwindow* window, int button, int action, int mods) {
    double xpos, ypos;
    if (action == GLFW_PRESS && button == GLFW_MOUSE_BUTTON_LEFT) {
        glfwSetInputMode(window,GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        mouseDisabled = true;
    } else if (action == GLFW_PRESS && button == GLFW_MOUSE_BUTTON_RIGHT) {
        glfwGetCursorPos(window, &xpos, &ypos);

        int winW = s_instance->camera.getResolution().x;
        int winH = s_instance->camera.getResolution().y;

        int x = (int)xpos;
        int y = winH - (int)ypos;

        GLuint index = 0;
        glReadPixels(x, y, 1, 1, GL_STENCIL_INDEX, GL_UNSIGNED_INT, &index);

        float depth = 1.0f;
        glReadPixels(x, y, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &depth);

        std::cout << "Mouse pick: index=" << index
                  << " depth=" << depth << std::endl;

        if (index > 0) {
            s_instance->scene->setSelected(index);
        }
        glm::vec3 screenPos(x, y, depth);
        glm::vec3 worldPos = glm::unProject(
            screenPos,
            s_instance->camera.getViewMatrix(),
            s_instance->camera.getProjectionMatrix(),
            glm::vec4(0, 0, winW, winH)
        );

        std::cout << "World position via unProject: "
                  << worldPos.x << ", "
                  << worldPos.y << ", "
                  << worldPos.z << std::endl;

    } else {
        glfwSetInputMode(window,GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        mouseDisabled = false;
    }
}

void Application::mouseMoveCallback(GLFWwindow* window, double xpos, double ypos) {
        if (!s_instance) return;
        if (s_instance->firstMouse) {
            s_instance->lastX = xpos;
            s_instance->lastY = ypos;
            s_instance->firstMouse = false;
        }
        float dx = xpos - s_instance->lastX;
        float dy = s_instance->lastY - ypos;

        s_instance->lastX = xpos;
        s_instance->lastY = ypos;

    if (mouseDisabled == true) {
        s_instance->camera.processMouseMovement(dx, dy);
    }
}

void Application::run(Scene* scn){
    while (!glfwWindowShouldClose(window)) {
        int width, height;
        glfwGetFramebufferSize(window, &width, &height);
        glViewport(0, 0, width, height);
        camera.setResolution(width, height);
        camera.setAspectRatio((float)width / height);

        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

        float currentFrame = glfwGetTime();

        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        scene = &scn[currentSceneIndex];
        scene->update(deltaTime);
        scene->activate();

        processInput(window);

        camera.update();

        scene->updateLights(camera);


        glEnable(GL_STENCIL_TEST);

        for (auto& object : scene->getObjects()) {
            auto shader = object->getShaderProgram();
            shader->setViewMatrix(camera.getViewMatrix());
            shader->setProjectionMatrix(camera.getProjectionMatrix());
            shader->setVec3("viewPos", camera.getPosition());
        }

        scene->draw();

        if (test == false) {
            scene->setCamera(&camera);
            test = true;
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glfwDestroyWindow(window);
    glfwTerminate();
}