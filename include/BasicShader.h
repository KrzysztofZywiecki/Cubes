#ifndef BASICSHADER_H
#define BASICSHADER_H

#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Shader.h"


class BasicShader : public Shader
{
public:
    BasicShader(const std::string vertex, const std::string geometry, const std::string fragment);
    ~BasicShader();
    void setCamera(glm::mat4 camera);
private:
    unsigned int cameraLocation;
};

#endif // BASICSHADER_H
