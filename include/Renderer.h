#pragma once

#include <vector>
#include <Eigen/Dense>
#include "Body.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>

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

        GLuint VAO, VBO;
        GLuint shaderProgram;

        void initShaders();
        GLuint compileShader(const char* source, GLenum type);
};