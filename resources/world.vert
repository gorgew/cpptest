#version 330 core

layout (std140) uniform CameraBlock 
{
    mat4 projection;
    mat4 view;
};
    
layout (location = 0) in vec3 in_Pos;
layout (location = 1) in vec2 in_TexCoord;

out vec3 vertColor;
out vec2 TexCoord;

uniform mat4 model;

void main() 
{
    gl_Position = projection * view * model * vec4(in_Pos, 1.0);
    TexCoord = in_TexCoord;
}