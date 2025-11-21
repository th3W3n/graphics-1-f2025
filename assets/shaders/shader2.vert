#version 460

layout (location = 0) in vec3 vPos;
out vec3 color;
uniform mat4 u_mvp;

void main()
{
    vec4 pos = vec4(vPos, 1.0);
    color = pos.xyz;
    gl_Position = u_mvp * pos;
}