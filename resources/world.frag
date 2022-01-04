#version 330 core
out vec4 FragColor;

in vec2 TexCoord;

uniform sampler2DArray iTexture;
uniform float layer;
uniform vec3 light_color;

const float ambient = 0.9;

void main()
{   

    //FragColor = result * texture(iTexture, vec3(TexCoord, layer));
    FragColor = texture(iTexture, vec3(TexCoord, layer));
}