#version 330 core
out vec4 FragColor;

in vec2 TexCoord;

uniform sampler2D iTexture;
uniform vec3 color; 

void main()
{   
    vec4 sampled = vec4(1.0, 1.0, 1.0, texture(iTexture, TexCoord).r);
    FragColor = vec4(color, 1.0) * sampled;
    //FragColor = texture(iTexture, TexCoord);
}