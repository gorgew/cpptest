#version 330 core
out vec4 FragColor;

in vec2 TexCoord;

uniform sampler2DArray iTexture;
uniform float layer;

void main()
{
    FragColor = texture(iTexture, vec3(TexCoord, layer));
    if (FragColor.a <= 0.1) {
        discard;
    }
}