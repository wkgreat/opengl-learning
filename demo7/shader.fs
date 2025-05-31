#version 330 core

in vec2 v_texcoord;
uniform sampler2D u_texture1;
uniform sampler2D u_texture2;

out vec4 FragColor;

void main() {
    FragColor = mix(texture(u_texture1, v_texcoord),texture(u_texture2, v_texcoord), 0.2);
    // FragColor = vec4(1.0, 0.5, 0.2, 1.0);
}