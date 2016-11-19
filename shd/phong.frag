#version 330 core

uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 lightPosition;
uniform vec3 cameraPosition;

in vec3 vPosition;
in vec3 vNormal;

out vec4 color;

void main() {
    /* Ambient light */
    vec3 ambient = 0.3 * lightColor;

    /* Diffuse light */
    vec3 norm = normalize(vNormal);
    vec3 lightDir = normalize(lightPosition - vPosition);

    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

    /* Specular light */
    float specularStrength = 0.5;

    vec3 viewDir = normalize(cameraPosition - vPosition);
    vec3 reflectDir = reflect(-lightDir, norm);

    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 128);
    vec3 specular = specularStrength * spec * lightColor;

    /* Output */
    vec3 result = (ambient + diffuse + specular) * objectColor;
	color = vec4(result, 1.0);
}
