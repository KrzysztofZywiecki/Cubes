#ifndef COMPUTESHADER_H
#define COMPUTESHADER_H

#include "Shader.h"


class ComputeShader : public Shader
{
public:
    ComputeShader(const std::string& file);
    ~ComputeShader();
    void setTime(float time);
private:
    unsigned int timeLocation;
};

#endif // COMPUTESHADER_H
