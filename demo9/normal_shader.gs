#version 330 core

layout(triangles) in;
layout(line_strip, max_vertices=2) out;

uniform mat4 u_modelMtx;
uniform mat4 u_viewMtx;
uniform mat4 u_projMtx;

in vec3 v_raw_normal[];
in vec4 v_raw_position[];

void main() {

    float factor = 0.5;

    vec4 center_pos = (v_raw_position[0]+v_raw_position[1]+v_raw_position[2]) / 3.0;
    vec3 center_norm = normalize(v_raw_normal[0]+v_raw_normal[1]+v_raw_normal[2]);
    vec4 target_pos = vec4(center_pos.xyz + center_norm * factor, 1.0);

    center_pos = u_projMtx * u_viewMtx * u_modelMtx * center_pos;
    target_pos = u_projMtx * u_viewMtx * u_modelMtx * target_pos;

    gl_Position = center_pos;
    EmitVertex();
    gl_Position = target_pos;
    EmitVertex();

}