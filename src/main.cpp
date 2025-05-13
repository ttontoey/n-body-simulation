#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <vector>
#include <iostream>
#include <fstream>
#include <iomanip>

#include <chrono>

#include "simulator/Particle.h"
#include "simulator/BaseSimMethod.h"
#include "simulator/NaivePairwise.h"
#include "simulator/VerletPairwise.h"


int FRAME = 0;
int NUM_PARTICLES = 200;
float G = 1E-5f;
float DT = 0.01f;

float COLOR_R = 1.0f;
float COLOR_G = 1.0f;
float COLOR_B = 1.0f;

std::vector<Particle> particles;
BaseSimMethod* engine = new VerletPairwise(G, DT, false);

std::vector<float> energyRecord;

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
    gl_PointSize = 2.0 * sqrt(aData.z); // Scale mass to size
}
)";

const char* fragmentShaderSource = R"(
#version 330 core
out vec4 FragColor;
uniform vec3 userColor;
void main() {
    FragColor = vec4(userColor, 1.0);
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
    std::cout << "Choose simulation method (NaivePairwise [0], VerletPairwise [1]): ";
    std::cin >> method;

    char enableCollision;
    std::cout << "Enable perfectly inelastic collision? [Y/N]: ";
    std::cin >> enableCollision;

    std::cout << "Enter RGB values (0.0 - 1.0):\n";
    std::cout << "R: "; std::cin >> COLOR_R;
    std::cout << "G: "; std::cin >> COLOR_G;
    std::cout << "B: "; std::cin >> COLOR_B;

    if (method == 0) {
        engine = new NaivePairwise(G, DT, enableCollision=='Y');
    } else if (method == 1) {
        engine = new VerletPairwise(G, DT, enableCollision=='Y');
    } else {
        std::cerr << "Invalid method type. Using NaivePairwise by default.\n";
        engine = new NaivePairwise(G, DT, enableCollision=='Y');
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
    GLint colorLocation = glGetUniformLocation(shaderProgram, "userColor");



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

        
        if (FRAME % 1000 == 0 || FRAME == 1){
            std::cout << "Frame #" << FRAME << " | Frame compute time: " << duration.count() << "s Particles: " << data.size() << " Energy: " << engine->getTotalEnergy() << std::endl;
        }

        energyRecord.push_back(engine->getTotalEnergy());

        // if (FRAME == 100000){
        //     std::ofstream outFile("energy_record.txt");
        //     if (outFile.is_open()) {
        //         for (size_t i = 0; i < energyRecord.size(); ++i) {
        //             outFile << std::setprecision(10) << energyRecord[i];
        //             if (i != energyRecord.size() - 1) outFile << ",";
        //         }
        //         outFile << std::endl;
        //         outFile.close();
        //         std::cout << "Energy record written to energy_record.txt\n";
        //     } else {
        //         std::cerr << "Failed to write energy record to file.\n";
        //     }
        //     return 0;
        // }

        glClear(GL_COLOR_BUFFER_BIT);
        glUseProgram(shaderProgram);
        glUniform3f(colorLocation, COLOR_R, COLOR_G, COLOR_B);

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
