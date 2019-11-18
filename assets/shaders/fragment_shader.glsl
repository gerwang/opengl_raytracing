#version 430 core

in vec3 fs_normal;
in vec2 fs_tex_pos;
in vec3 FragPos;

layout(location = 0) out vec4 out_color;

uniform sampler2D textureMap;
uniform sampler2D normalMap;

uniform vec3 lightPos;
uniform vec3 viewPos;
uniform vec3 lightColor;


void main() {
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * lightColor;

    vec3 lightDir = normalize(lightPos - FragPos);
    vec3 norm = normalize(fs_normal);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

    float specularStrength = 0.5;
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularStrength * spec * lightColor;

    vec3 objectColor = texture(textureMap, fs_tex_pos).xyz;

    vec3 result = (ambient + diffuse + specular) * objectColor;
    out_color = vec4(result, 1.0);
}
