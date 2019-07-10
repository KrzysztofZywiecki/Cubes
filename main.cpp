#include <iostream>
#include <glad/glad.h>
#include <glfw/glfw3.h>
#include "ComputeShader.h"
#include "BasicShader.h"
#include <ctime>
#include <cstdlib>
#include <vector>

void callback(GLenum source,
	GLenum type,
	GLuint id,
	GLenum severity,
	GLsizei length,
	const GLchar* message,
	void* userParam) {
	std::cout << "OpenGL Error" << std::endl;
	std::cout << "[" << id << "]:" << message << " (" << type << ")"<<std::endl;
}

struct Vector4f
{
    float x, y, z, w;
};

class Cubes
{
public:
    void addCube(Vector4f initialVelocity);
    std::vector<Vector4f>& getPositions();
    std::vector<Vector4f>& getVelocities();
    std::vector<char>& getToReposition();
private:
    std::vector<Vector4f> positions;
    std::vector<Vector4f> velocities;
    std::vector<char> toReposition;
};

std::vector<Vector4f>& Cubes::getPositions()
{
    return positions;
}

std::vector<Vector4f>& Cubes::getVelocities()
{
    return velocities;
}

std::vector<char>& Cubes::getToReposition()
{
    return toReposition;
}

void Cubes::addCube(Vector4f initialVelocity)
{
    positions.push_back({0.0f, 0.0f, 0.0f, 0.0f});
    velocities.push_back(initialVelocity);
    toReposition.push_back(false);
}

int main()
{
    srand(time(nullptr));
    glfwInit();
    GLFWwindow* window = glfwCreateWindow(1920, 1080, "magister", nullptr, nullptr);
    glfwMakeContextCurrent(window);

    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_DEBUG_OUTPUT);
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
    glDebugMessageCallback((GLDEBUGPROC)callback, nullptr);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    float vertices[] = {0.0f, 0.0f, 0.0f,
                    1.0f, 0.0f, 0.0f,
                    1.0f, 0.0f, 1.0f,
                    0.0f, 0.0f, 1.0f,

                    0.0f, 1.0f, 0.0f,
                    1.0f, 1.0f, 0.0f,
                    1.0f, 1.0f, 1.0f,
                    0.0f, 1.0f, 1.0f};

    unsigned int indices[] = {0, 1, 4, 4, 5, 1,
                            3, 2, 7, 7, 2, 6,
                            1, 6, 2, 1, 5, 6,
                            0, 3, 4, 4, 3, 7,
                            0, 3, 1, 3, 2, 1,
                            4, 6, 7, 4, 5, 6};

    unsigned int vao;
    unsigned int vbo, ebo;

    glCreateVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, false, 3*sizeof(float), (void*)(0));
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glGenBuffers(1, &ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glm::mat4 perspective = glm::perspective(glm::radians(90.0f), 1920.0f/1080.0f, 0.1f, 50.0f);
    glm::mat4 looking = glm::lookAt(glm::vec3(15.0f, 10.0f, 15.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

    BasicShader shader("Shaders/cube.vert", "Shaders/cube.geom", "Shaders/cube.frag");
    ComputeShader cShader("Shaders/terrain.comp");

    Cubes cubes;

    for(int i=0; i<1024; i++)
    {
        Vector4f vel = {(double)(rand()/(double)RAND_MAX) * 20.0f - 10.0f, (double)(rand()/(double)RAND_MAX) * 30.0f, (double)(rand()/(double)RAND_MAX) * 20.0f - 10.0f, 0.0f};
        std::cout<<vel.x<<"   "<<vel.y<<"  "<<vel.z<<std::endl;
        cubes.addCube(vel);
    }

    unsigned int positions;
    glGenBuffers(1, &positions);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, positions);
    glBufferData(GL_SHADER_STORAGE_BUFFER, cubes.getPositions().size()*sizeof(Vector4f), cubes.getPositions().data(), GL_STATIC_DRAW);

    unsigned int velocities;
    glGenBuffers(1, &velocities);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, velocities);
    glBufferData(GL_SHADER_STORAGE_BUFFER, cubes.getVelocities().size()*sizeof(Vector4f), cubes.getVelocities().data(), GL_STATIC_DRAW);

    unsigned int removing;
    glGenBuffers(1, &removing);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, removing);
    glBufferData(GL_SHADER_STORAGE_BUFFER, cubes.getToReposition().size()*sizeof(char), cubes.getToReposition().data(), GL_STATIC_DRAW);

    while(!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shader.start();
        glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, positions);
        shader.setCamera(perspective * looking);
        glDrawElementsInstanced(GL_TRIANGLES, sizeof(indices)/sizeof(unsigned int), GL_UNSIGNED_INT, nullptr, 1024);
        shader.stop();

        cShader.start();
        glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, positions);
        glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, velocities);
        glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, removing);
        cShader.setTime(0.0009f);
        glDispatchCompute(cubes.getToReposition().size()/16, 1, 1);
        cShader.stop();
        glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);

        glfwPollEvents();
        glfwSwapBuffers(window);
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
