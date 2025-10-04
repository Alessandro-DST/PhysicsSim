#pragma once

#include <vector>
#include <Eigen/Dense>
#include "Body.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Camera.h"

class Renderer {
    public:
        Renderer(unsigned int width, unsigned int height, const char* title);
        ~Renderer();

        void render(const std::vector<Body>& bodies);

        bool shouldClose() const;
        void swapBuffers();
        void pollEvents();
    
    private:
        GLFWwindow* window;
        unsigned int width, height;
        
        GLuint VAO, VBO;
        GLuint shaderProgram;

        GLuint gridVAO, gridVBO;
        GLuint gridShaderProgram;

        Camera camera;
        float lastFrameTime = 0.0f;
        float deltaTime = 0.0f;

        bool firstMouseLeft = true;
        bool firstMouseMiddle = true;
        bool leftMousePressed = false;
        bool middleMousePressed = false;
        float lastX = 400.0f;
        float lastY = 300.0f;

        bool showGrid = true;
        bool showTrails = true; // TODO
        float zoom = 1.0f;
        glm::vec2 pan = glm::vec2(0.0f);

        void initShaders();
        GLuint compileShader(const char* source, GLenum type);
        void setUniformMat4(GLuint program, const std::string& name, const glm::mat4& mat);

        void processInput();
        static void framebufferSizeCallback(GLFWwindow* window, int width, int height);
        static void mouseCallback(GLFWwindow* window, double xpos, double ypos);
        static void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
        static void scrollCallback(GLFWwindow* window, double xoffset, double yoffset);

};