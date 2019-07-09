#include <iostream>
#include <glad/glad.h>
#include <glfw/glfw3.h>
#include "ComputeShader.h"

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

int main()
{
    glfwInit();
    GLFWwindow* window = glfwCreateWindow(800, 600, "magister", nullptr, nullptr);
    glfwMakeContextCurrent(window);

    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

    glEnable(GL_DEBUG_OUTPUT);
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
    glDebugMessageCallback((GLDEBUGPROC)callback, nullptr);


    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    float data[] = {1.0f, 2.0f, 3.0f, 4.0f};
    unsigned int buffer;
    glGenBuffers(1, &buffer);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, buffer);
    glBufferData(GL_SHADER_STORAGE_BUFFER, 4*sizeof(float), data, GL_DYNAMIC_COPY);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);

    ComputeShader shader("Shaders/terrain.comp");
    shader.start();
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, buffer);
    glDispatchCompute(1, 1, 1);
    shader.stop();

    float* data2;
    data2 = reinterpret_cast<float*>(glMapBuffer(GL_SHADER_STORAGE_BUFFER, GL_READ_ONLY));
    for(int i=0; i<4; i++)
    {
        std::cout<<"["<<data2[i]<<"] ";
    }
    glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);

    while(!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT);



        glfwPollEvents();
        glfwSwapBuffers(window);
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
