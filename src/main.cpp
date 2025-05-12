#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <vector>
#include <iostream>
#include <chrono>

#include "simulator/Particle.h"
#include "simulator/BaseSimMethod.h"
#include "simulator/NaivePairwise.h"
#include "simulator/VerletPairwise.h"

int FRAME = 0;
int NUM_PARTICLES = 700;
float G = 1E-5f;
float DT = 0.01f;

std::vector<Particle> particles;

BaseSimMethod* engine = new NaivePairwise(G, DT);

void initialize() {
    engine->initialize(NUM_PARTICLES);
}

void simulate() {
    engine->simulate();
}

const char* vertexShaderSource = R"(
#version 330 core
layout(location = 0) in vec3 aData; // x, y = position; z = mass

void main() {
    gl_Position = vec4(aData.xy, 0.0, 1.0);
    gl_PointSize = sqrt(aData.z); // Scale mass to size
}
)";

const char* fragmentShaderSource = R"(
#version 330 core
out vec4 FragColor;
void main() {
    FragColor = vec4(1.0);
}
)";

GLuint compileShader(GLenum type, const char* src) {
    GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, &src, NULL);
    glCompileShader(shader);
    return shader;
}

void getUserInput() {
    std::cout << "Enter number of particles: ";
    std::cin >> NUM_PARTICLES;
    std::cout << "Enter gravitational constant (G): ";
    std::cin >> G;
    std::cout << "Enter time step (DT): ";
    std::cin >> DT;

    int method;
    std::cout << "Choose simulation method [0] for NaivePairwise, [1] for VerletPairwise): ";
    std::cin >> method;

    if (method == 0) {
        engine = new NaivePairwise(G, DT);
    } else if (method == 1) {
        engine = new VerletPairwise(G, DT);
    } else {
        std::cerr << "Invalid method type. Using NaivePairwise by default.\n";
        engine = new NaivePairwise(G, DT);
    }
}

int main() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    GLFWwindow* window = glfwCreateWindow(800, 800, "N-Body Simulation", NULL, NULL);
    glfwMakeContextCurrent(window);
    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

    GLuint vao, vbo;
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);

    GLuint vs = compileShader(GL_VERTEX_SHADER, vertexShaderSource);
    GLuint fs = compileShader(GL_FRAGMENT_SHADER, fragmentShaderSource);
    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vs);
    glAttachShader(shaderProgram, fs);
    glLinkProgram(shaderProgram);


    glEnable(GL_PROGRAM_POINT_SIZE);

    getUserInput();
    initialize();

    while (!glfwWindowShouldClose(window)) {

        auto start = std::chrono::high_resolution_clock::now();
        simulate();
        FRAME += 1;
        auto end = std::chrono::high_resolution_clock::now();

        std::chrono::duration<double> duration = end - start;
        std::vector<glm::vec2> positions;
        std::vector<glm::vec3> data;
        for (const auto& p : engine->getParticles())
            data.emplace_back(p.getPosition(), p.getMass());

        if (FRAME % 1000 == 0){
            std::cout << "Frame #" << FRAME << " | Frame compute time: " << duration.count() << " Particles: " << data.size() << std::endl;
        }

        glClear(GL_COLOR_BUFFER_BIT);
        glUseProgram(shaderProgram);
        glBindVertexArray(vao);

        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * data.size(), data.data(), GL_DYNAMIC_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);
        glEnableVertexAttribArray(0);

        glDrawArrays(GL_POINTS, 0, data.size());

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
