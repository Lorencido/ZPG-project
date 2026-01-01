#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Scene.h"
#include "Camera.h"
#include <iostream>

class Application {
    GLFWwindow* window;
    Scene* scene;
    Camera camera;
    float lastX, lastY;
    bool firstMouse;
public:
    Application();
    void run(Scene* scn);

    static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
    static void keyMouseCallback(GLFWwindow* window, int button, int action, int mods);
    static void mouseMoveCallback(GLFWwindow* window, double xpos, double ypos);

    static void processInput(GLFWwindow* window);

};