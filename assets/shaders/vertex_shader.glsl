#version 430 core

layout(location = 0) in vec3 in_position;
layout(location = 1) in vec2 in_tex_pos;
layout(location = 2) in vec3 in_normal;
layout (location = 3) in vec3 tangent;
layout (location = 4) in vec3 bitangent;

out vec3 fs_normal;
out vec2 fs_tex_pos;
out vec3 FragPos;
out vec4 FragPosLightSpace;
out vec3 TangentLightPos;
out vec3 TangentViewPos;
out vec3 TangentFragPos;

uniform mat4 mvpMat;
uniform mat4 normalMat;
uniform mat4 modelMat;
uniform mat4 lightSpaceMatrix;
uniform vec3 lightPos;
uniform vec3 viewPos;

void main() {
    gl_Position = mvpMat * vec4(in_position, 1.0f);
    fs_tex_pos = in_tex_pos;
    FragPos = vec3(modelMat * vec4(in_position, 1.0f));
    FragPosLightSpace = lightSpaceMatrix * vec4(FragPos, 1.0);

    mat3 normalMatrix = mat3(normalMat);
    vec3 T = normalize(normalMatrix * tangent);
    vec3 B = normalize(normalMatrix * bitangent);
    vec3 N = normalize(normalMatrix * in_normal);

    mat3 TBN = transpose(mat3(T, B, N));
    TangentLightPos = TBN * lightPos;
    TangentViewPos  = TBN * viewPos;
    TangentFragPos  = TBN * FragPos;
    fs_normal = in_normal;
}