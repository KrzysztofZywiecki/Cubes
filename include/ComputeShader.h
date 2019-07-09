#ifndef COMPUTESHADER_H
#define COMPUTESHADER_H

#include "Shader.h"


class ComputeShader : public Shader
{
public:
    ComputeShader(const std::string& file);
    ~ComputeShader();

private:

};

#endif // COMPUTESHADER_H
