#include "Shader.h"

Shader::Shader()
{
    shaderID = glCreateProgram();
}

Shader::~Shader()
{
    glDeleteProgram(shaderID);
}

unsigned int compileShader(const std::string& title, unsigned int type)
{
    std::ifstream file(title);
    if(!file.good())
    {
        std::cout<<"File read error :"<<title;
        return 0;
    }

    unsigned int shader = glCreateShader(type);
    std::string line;
    std::string source;

    while(!file.eof())
    {
        getline(file, line);
        source.append(line + '\n');
    }
    const char* src = source.c_str();
    glShaderSource(shader, 1, &src, nullptr);
    glCompileShader(shader);

    int compileStatus;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &compileStatus);
    if(compileStatus != GL_TRUE)
    {
        char message[2048];
        glGetShaderInfoLog(shader, 2048, nullptr, message);
        std::cout<<message;
    }
    return shader;
}

void Shader::start()
{
    glUseProgram(shaderID);
}

void Shader::stop()
{
    glUseProgram(0);
}
