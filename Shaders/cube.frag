#version 450 core

out vec4 color;

in vec3 normal;

vec3 col = vec3(0.8f, 0.8f, 0.8f);
vec4 ambient = vec4(0.2f, 0.2f, 0.2f, 1.0f);

void main()
{
    vec3 light = normalize(vec3(1, 2, -0.5));

    color = max(vec4(max(dot(normal, light), 0) * col, 1.0f), ambient);
}
