#include <GLFW/glfw3.h>
#include <iostream>

int main() {
    if (!glfwInit()) {
        std::cerr << "GLFW init failed\n";
        return -1;
    }
    GLFWwindow* window = glfwCreateWindow(100, 100, "Test", nullptr, nullptr);
    if (!window) {
        std::cerr << "Window creation failed\n";
        glfwTerminate();
        return -1;
    }
    std::cout << "GLFW window created\n";
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
