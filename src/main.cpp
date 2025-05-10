#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <vector>
#include <iostream>

const int NUM_BODIES = 200;
const float G = 0.001f;
const float DT = 0.01f;

struct Body {
    glm::vec2 pos;
    glm::vec2 vel;
    float mass;
};

std::vector<Body> bodies;

void initBodies() {
    for (int i = 0; i < NUM_BODIES; ++i) {
        Body b;
        b.pos = glm::vec2(rand() % 1000 / 500.0f - 1.0f, rand() % 1000 / 500.0f - 1.0f);
        b.vel = glm::vec2(0.0f);
        b.mass = 1.0f;
        bodies.push_back(b);
    }
}

void simulate() {
    for (int i = 0; i < NUM_BODIES; ++i) {
        glm::vec2 force(0.0f);
        for (int j = 0; j < NUM_BODIES; ++j) {
            if (i == j) continue;
            glm::vec2 dir = bodies[j].pos - bodies[i].pos;
            float distSqr = glm::dot(dir, dir) + 1e-4f;
            float strength = G * bodies[i].mass * bodies[j].mass / distSqr;
            force += strength * glm::normalize(dir);
        }
        bodies[i].vel += DT * force / bodies[i].mass;
    }

    for (int i = 0; i < NUM_BODIES; ++i) {
        bodies[i].pos += DT * bodies[i].vel;
    }
}

const char* vertexShaderSource = R"(
#version 330 core
layout(location = 0) in vec2 aPos;
void main() {
    gl_Position = vec4(aPos, 0.0, 1.0);
    gl_PointSize = 3.0;
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

    initBodies();

    while (!glfwWindowShouldClose(window)) {
        simulate();

        std::vector<glm::vec2> positions;
        for (const auto& b : bodies)
            positions.push_back(b.pos);

        glClear(GL_COLOR_BUFFER_BIT);
        glUseProgram(shaderProgram);
        glBindVertexArray(vao);

        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2) * positions.size(), positions.data(), GL_DYNAMIC_DRAW);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2), (void*)0);
        glEnableVertexAttribArray(0);

        glDrawArrays(GL_POINTS, 0, NUM_BODIES);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
