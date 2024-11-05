#version 460 core

in vec3 Normal;
in vec3 LightDir;

out vec4 FragColor;

uniform vec3 objectColor;
uniform vec3 lightColor;
uniform int toonLevels = 4;

uniform sampler2D texture_diffuse1;

void main()
{

    // Normalize vectors
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(LightDir);

    // Calculate diffuse lighting intensity
    float intensity = max(dot(norm, lightDir), 0.0);

    // Quantize the lighting by dividing it into discrete bands
    float quantizedIntensity = floor(intensity * toonLevels) / toonLevels;

    // Apply the quantized intensity to the object color
    vec3 color = quantizedIntensity * objectColor;

    FragColor = vec4(lightColor * color, 1.0);
    // FragColor = texture(texture_diffuse1, TexCoords);
    // FragColor = vec4(lightColor * objectColor, 1.0);
}
