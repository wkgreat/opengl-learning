#version 330 core
layout (location = 0) in vec3 a_position;
layout (location = 1) in vec2 a_texcoord;

uniform mat4 u_modelMtx;

out vec2 v_texcoord;

void main() {
    gl_Position = u_modelMtx * vec4(a_position, 1.0);
    v_texcoord = a_texcoord;
}