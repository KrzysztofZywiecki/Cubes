#include "ComputeShader.h"

ComputeShader::ComputeShader(const std::string& file)
{
    unsigned int shader = compileShader(file, GL_COMPUTE_SHADER);
    glAttachShader(shaderID, shader);
    glLinkProgram(shaderID);
}

ComputeShader::~ComputeShader()
{

}
