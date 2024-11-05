#version 460 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 3) in vec2 aTexCoords;

out vec3 Normal;
out vec3 LightDir;

uniform vec3 lightPosition;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    vec4 worldPosition = model * vec4(aPos, 1.0);
    LightDir = normalize(lightPosition - worldPosition.xyz);
    Normal = mat3(transpose(inverse(model))) * aNormal;

    gl_Position = projection * view * worldPosition;
    // TexCoords = aTexCoords;    
    // gl_Position = projection * view * model * vec4(aPos, 1.0);
} 
