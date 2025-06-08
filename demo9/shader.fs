#version 330 core

in vec2 v_texcoord;
uniform sampler2D u_texture;

out vec4 FragColor;

void main() {
    FragColor = texture(u_texture, v_texcoord);
    // FragColor = vec4(0.0,0.0,1.0,1.0);
}