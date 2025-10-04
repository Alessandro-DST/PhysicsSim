#include "Renderer.h"
#include <iostream>
#include <fstream>
#include <sstream>

Renderer::Renderer(unsigned int width, unsigned int height, const char* title)
    : camera(glm::vec3(0.0f, 0.0f, 3.0f)) {

        if (!glfwInit()){
            std::cerr << "Failed to initialize GLFW\n";
            exit(-1);
        }

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        
        this->width = width;
        this->height = height;
        window = glfwCreateWindow(width, height, title, nullptr, nullptr);
        if (!window){
            std::cerr << "Failed to create GLFW window\n";
            glfwTerminate();
            exit(-1);
        }

        glfwSetWindowUserPointer(window, this);
        glfwSetMouseButtonCallback(window, mouseButtonCallback);
        glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);
        glfwSetCursorPosCallback(window, mouseCallback);
        glfwSetScrollCallback(window, scrollCallback);



        std::cout << "Window created at " << width << "x" << height << "\n";

        glfwMakeContextCurrent(window);
        glfwSwapInterval(1);

        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
            std::cerr << "Failed to initialize GLAD\n";
            exit(-1);
        }

        glEnable(GL_DEPTH_TEST);
        glEnable(GL_PROGRAM_POINT_SIZE);
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

        // Setup VBO/VAO
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        // Initially empty buffer, will update every frame
        glBufferData(GL_ARRAY_BUFFER, 0, nullptr, GL_DYNAMIC_DRAW);
        // size = 3 --> Each vertex has 3 components : x,y,z
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        initShaders();
}

Renderer::~Renderer() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shaderProgram);
    glfwDestroyWindow(window);
    glfwTerminate();
}

// Render Function
void Renderer::render(const std::vector<Body>& bodies){
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Flatten Vector3d into contingous vector [x1, y1, z1, x2, y2, z2, ...]
    std::vector<float> positions;
    positions.reserve(bodies.size() * 3);
    for (const auto& b : bodies){
        positions.push_back(b.position.x());
        positions.push_back(b.position.y());
        positions.push_back(b.position.z());
    }

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, positions.size() * sizeof(float), positions.data(), GL_DYNAMIC_DRAW);

    glUseProgram(shaderProgram);

    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 view = camera.GetViewMatrix();
    glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom),
        (float)width / (float)height, 0.1f, 100.0f);

    setUniformMat4(shaderProgram, "model", model);
    setUniformMat4(shaderProgram, "view", view);
    setUniformMat4(shaderProgram, "projection", projection);

    glBindVertexArray(VAO);
    glDrawArrays(GL_POINTS, 0, bodies.size());
}

// Window Functions
bool Renderer::shouldClose() const { return glfwWindowShouldClose(window); }
void Renderer::swapBuffers() { glfwSwapBuffers(window); }

void Renderer::pollEvents(){
    float currentTime = glfwGetTime();
    deltaTime = currentTime - lastFrameTime;
    lastFrameTime = currentTime;

    processInput();
    glfwPollEvents(); }

// Shader Helper Functions
GLuint Renderer::compileShader(const char* source, GLenum type){
    GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, &source, nullptr);
    glCompileShader(shader);

    int success;
    char infoLog[512];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success){
        glGetShaderInfoLog(shader, 512, nullptr, infoLog);
        std::cerr << "Shader compilation error: " << infoLog << "\n";
    }

    return shader;
}

void Renderer::initShaders() {
    // load strings for now as a test
    const char* vertexSrc = R"(
        #version 330 core
        layout(location = 0) in vec3 aPos;

        uniform mat4 model;
        uniform mat4 view;
        uniform mat4 projection;

        void main() {
            gl_Position = projection * view * model * vec4(aPos, 1.0);
            gl_PointSize = 5.0;
        }
    )";

    const char* fragmentSrc = R"(
        #version 330 core
        out vec4 FragColor;
        void main() {
            FragColor = vec4(1.0, 0.0, 0.0, 1.0);
        }
    )";

    GLuint vertex = compileShader(vertexSrc, GL_VERTEX_SHADER);
    GLuint fragment = compileShader(fragmentSrc, GL_FRAGMENT_SHADER);

    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertex);
    glAttachShader(shaderProgram, fragment);
    glLinkProgram(shaderProgram);

    int success;
    char infoLog[512];
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success){
        glGetProgramInfoLog(shaderProgram, 512, nullptr, infoLog);
        std::cerr << "Shader linking error: " << infoLog << "\n";
    }

    glDeleteShader(vertex);
    glDeleteShader(fragment);
}

void Renderer::framebufferSizeCallback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
    Renderer* renderer = static_cast<Renderer*>(glfwGetWindowUserPointer(window));
    if (renderer){
        renderer->width = width;
        renderer->height = height;
    }
}

void Renderer::setUniformMat4(GLuint program, const std::string& name, const glm::mat4& mat) {
    GLuint loc = glGetUniformLocation(program, name.c_str());
    if (loc == -1){
        std::cerr << "Warning: uniform " << name << " not found!\n";
    }

    glUniformMatrix4fv(loc, 1, GL_FALSE, &mat[0][0]);
}

void Renderer::processInput() {
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime);
}

void Renderer::mouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
    Renderer* renderer = static_cast<Renderer*>(glfwGetWindowUserPointer(window));

    if (button == GLFW_MOUSE_BUTTON_LEFT){
        renderer->leftMousePressed = (action == GLFW_PRESS);
        if (action == GLFW_PRESS){
            renderer->firstMouseLeft = true;
        }
    }
    if (button == GLFW_MOUSE_BUTTON_MIDDLE){
        renderer->middleMousePressed = (action == GLFW_PRESS);
        if (action == GLFW_PRESS){
            renderer->firstMouseMiddle = true;
        }
    }
}

void Renderer::mouseCallback(GLFWwindow* window, double xpos, double ypos) {
    Renderer* renderer = static_cast<Renderer*>(glfwGetWindowUserPointer(window));

    if (renderer->leftMousePressed) {
        if (renderer->firstMouseLeft) {
            renderer->lastX = xpos;
            renderer->lastY = ypos;
            renderer->firstMouseLeft = false;
        }
        float xoffset = renderer->lastX - xpos;
        float yoffset = ypos - renderer->lastY;
        renderer->lastX = xpos;
        renderer->lastY = ypos;
        renderer->camera.ProcessMouseMovement(xoffset, yoffset);
    }
    else if (renderer->middleMousePressed) {
        if (renderer->firstMouseMiddle) {
            renderer->lastX = xpos;
            renderer->lastY = ypos;
            renderer->firstMouseMiddle = false;
        }
        float xoffset = renderer->lastX - xpos;
        float yoffset = ypos - renderer->lastY;
        renderer->lastX = xpos;
        renderer->lastY = ypos;
        renderer->camera.ProcessMousePan(xoffset, yoffset);
    }
    else {
        renderer->firstMouseLeft = true;
        renderer->firstMouseMiddle = true;
    }
}


void Renderer::scrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
    Renderer* renderer = static_cast<Renderer*>(glfwGetWindowUserPointer(window));
    renderer->camera.ProcessMouseScroll(yoffset);
}
