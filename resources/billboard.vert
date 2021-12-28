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
    vec3 camera_right = vec3(view[0][0], view[1][0], view[2][0]);
    vec3 camera_up = vec3(view[0][1], view[1][1], view[2][1]);
    vec3 out_pos = camera_right * in_Pos.x + camera_up * in_Pos.y;
    gl_Position = projection * view * model * vec4(out_pos, 1.0);
    TexCoord = in_TexCoord;
}