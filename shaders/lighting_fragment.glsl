#version 460 core

out vec4 FragColor;

in vec3 normal;
in vec3 fragPosition;

uniform vec3 cameraPosition;

// was object color
struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
}; 

// was light strengh
struct Light {
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform Material material;
uniform Light light;

void main() {
    vec3 norm = normalize(normal);
    vec3 lightDirection = normalize(light.position - fragPosition);
    float diff = max(dot(norm, lightDirection), 0.0);
    vec3 diffuse = light.diffuse * diff * material.diffuse;

    vec3 ambient = light.ambient * material.ambient;

    vec3 cameraDirection = normalize(cameraPosition - fragPosition);
    vec3 reflectDirection = reflect(-lightDirection, norm);

    float spec = pow(max(dot(cameraDirection, reflectDirection), 0.0), material.shininess);
    vec3 specular = light.specular * spec * material.specular;

    vec3 result = (ambient + diffuse + specular);
    FragColor = vec4(result, 1.0);
}
