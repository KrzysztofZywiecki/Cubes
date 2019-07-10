#version 450 core

layout(triangles) in;
layout(triangle_strip, max_vertices = 3) out;

out primitiveData
{
    vec3 normal;
};

in vec3 worldPosition[];

void main()
{
    vec3 vector1 = gl_in[1].gl_Position.xyz - gl_in[0].gl_Position.xyz;
    vec3 vector2 = gl_in[2].gl_Position.xyz - gl_in[0].gl_Position.xyz;
    vec3 norm = normalize(cross(vector1, vector2));

    gl_Position = gl_in[0].gl_Position;
    normal = norm;
    EmitVertex();

    gl_Position = gl_in[1].gl_Position;
    normal = norm;
    EmitVertex();

    gl_Position = gl_in[2].gl_Position;
    normal = norm;
    EmitVertex();
    EndPrimitive();
}
