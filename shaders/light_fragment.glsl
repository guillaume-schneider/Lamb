#version 460 core

out vec4 FragColor;
in vec2 TexCoord;

uniform sampler2D texture0;

void main()
{
    FragColor = texture(texture0, TexCoord);
    // FragColor = vec4(1.0);
}
