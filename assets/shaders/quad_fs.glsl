#version 430 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D texMap;

void main()
{
    FragColor = texture(texMap, TexCoords);// orthographic
}