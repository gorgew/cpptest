#version 330 core
    
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;

out vec3 vertColor;
out vec2 TexCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform vec2 size;

void main() 
{
    gl_Position = projection * view * model * vec4(aPos.x * size.x, aPos.y * size.y, aPos.z, 1.0);
    TexCoord = aTexCoord;
}