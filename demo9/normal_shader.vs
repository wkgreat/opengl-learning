#version 330 core
layout (location = 0) in vec3 a_position;
layout (location = 1) in vec3 a_normal;
layout (location = 2) in vec2 a_texcoord;

uniform mat4 u_modelMtx;
uniform mat4 u_viewMtx;
uniform mat4 u_projMtx;

out vec3 v_raw_normal;
out vec4 v_raw_position;

void main() {
    gl_Position = u_projMtx * u_viewMtx * u_modelMtx * vec4(a_position, 1.0);
    v_raw_position = vec4(a_position, 1.0);
    v_raw_normal = a_normal;
}