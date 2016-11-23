#version 330 core

struct Material {
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
};

uniform Material material;

struct Light {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    vec3 position;
};

uniform Light light;

uniform vec3 cameraPosition;

in vec3 vPosition;
in vec3 vNormal;
in vec2 vUV;

out vec4 color;

void main() {
    /* Ambient light */
    vec3 ambient = texture(material.diffuse, vUV).rgb * light.ambient;

    /* Diffuse light */
    vec3 norm = normalize(vNormal);
    vec3 lightDir = normalize(light.position - vPosition);

    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * texture(material.diffuse, vUV).rgb * light.diffuse;

    /* Specular light */
    vec3 viewDir = normalize(cameraPosition - vPosition);
    vec3 reflectDir = reflect(-lightDir, norm);

    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = spec * texture(material.specular, vUV).rgb * light.specular;

    /* Output */
	color = vec4(ambient + diffuse + specular, 1.0);
}
