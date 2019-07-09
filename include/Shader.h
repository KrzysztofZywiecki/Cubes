#ifndef SHADER_H
#define SHADER_H

#include <iostream>
#include <fstream>
#include <glad/glad.h>

unsigned int compileShader(const std::string& title, unsigned int type);

class Shader
{
public:
    Shader();
    ~Shader();

    void start();
    void stop();
protected:
    unsigned int shaderID;
};

#endif // SHADER_H
