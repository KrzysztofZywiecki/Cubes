#version 450 core

layout(location = 0) in vec3 position;

layout(std430, binding = 2) buffer entities
{
    vec4 offset[];
};

out vec3 worldPosition;

uniform mat4 camera;

void main()
{
    worldPosition = position + offset[gl_InstanceID].xyz;
    gl_Position = camera * (vec4(position, 1.0f) + offset[gl_InstanceID]);
}
