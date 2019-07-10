#include "BasicShader.h"

BasicShader::BasicShader(const std::string vertex, const std::string geometry, const std::string fragment)
{
    unsigned int vertexShader = compileShader(vertex, GL_VERTEX_SHADER);
    unsigned int fragmentShader = compileShader(fragment, GL_FRAGMENT_SHADER);
    unsigned int geometryShader = compileShader(geometry, GL_GEOMETRY_SHADER);

    glAttachShader(shaderID, vertexShader);
    glAttachShader(shaderID, fragmentShader);
    glAttachShader(shaderID, geometryShader);
    glLinkProgram(shaderID);
    cameraLocation = glGetUniformLocation(shaderID, "camera");
}

BasicShader::~BasicShader()
{
    //dtor
}

void BasicShader::setCamera(glm::mat4 camera)
{
    glUniformMatrix4fv(cameraLocation, 1, false, glm::value_ptr(camera));
}
