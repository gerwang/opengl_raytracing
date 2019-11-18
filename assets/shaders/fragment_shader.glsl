#version 430 core

in vec3 fs_normal;
in vec2 fs_tex_pos;
in vec3 FragPos;
in vec4 FragPosLightSpace;
in vec3 TangentLightPos;
in vec3 TangentViewPos;
in vec3 TangentFragPos;

layout(location = 0) out vec4 out_color;

uniform sampler2D textureMap;
uniform sampler2D normalMap;
uniform sampler2D shadowMap;

uniform vec3 lightColor;
uniform vec3 lightPos;

// code adapted from https://learnopengl.com/code_viewer_gh.php?code=src/5.advanced_lighting/3.1.3.shadow_mapping/3.1.3.shadow_mapping.fs
float ShadowCalculation(vec4 fragPosLightSpace)
{
    // perform perspective divide
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    // transform to [0,1] range
    projCoords = projCoords * 0.5 + 0.5;
    // get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
    float closestDepth = texture(shadowMap, projCoords.xy).r;
    // get depth of current fragment from light's perspective
    float currentDepth = projCoords.z;
    // calculate bias (based on depth map resolution and slope)
    vec3 normal = normalize(fs_normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    float bias = max(0.05 * (1.0 - dot(normal, lightDir)), 0.005);
    // check whether current frag pos is in shadow
    // float shadow = currentDepth - bias > closestDepth  ? 1.0 : 0.0;
    // PCF
    float shadow = 0.0;
    vec2 texelSize = 1.0 / textureSize(shadowMap, 0);
    for (int x = -1; x <= 1; ++x)
    {
        for (int y = -1; y <= 1; ++y)
        {
            float pcfDepth = texture(shadowMap, projCoords.xy + vec2(x, y) * texelSize).r;
            shadow += currentDepth - bias > pcfDepth  ? 1.0 : 0.0;
        }
    }
    shadow /= 9.0;

    //     keep the shadow at 0.0 when outside the far_plane region of the light's frustum.
    if (projCoords.z > 1.0) {
        shadow = 0.0;
    }

    return shadow;
}

void main() {
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * lightColor;

    vec3 lightDir = normalize(TangentLightPos - TangentFragPos);

    // Obtain normal from normal map in range [0,1]
    vec3 norm = texture(normalMap, fs_tex_pos).rgb;
    // Transform normal vector to range [-1,1]
    norm = normalize(norm * 2.0 - 1.0);// this normal is in tangent space

    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

    float specularStrength = 0.5;
    vec3 viewDir = normalize(TangentViewPos - TangentFragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularStrength * spec * lightColor;

    vec3 objectColor = texture(textureMap, fs_tex_pos).xyz;

    float shadow = ShadowCalculation(FragPosLightSpace);
    vec3 result = (ambient + (1.0 - shadow) * (diffuse + specular)) * objectColor;
    out_color = vec4(result, 1.0f);
}
