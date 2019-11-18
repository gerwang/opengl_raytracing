#version 430 core

in vec3 fs_normal;
in vec2 fs_tex_pos;

layout(location = 0) out vec4 out_color;

void main() {
    //    out_color = texture(corresTexture, fs_tex_pos);
    out_color = vec4(0.75);
}
