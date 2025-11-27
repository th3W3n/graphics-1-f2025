#version 460 core

in vec2 tcoord;
out vec4 fragColor;
uniform sampler2D tex1;
// uniform sampler2D tex2;

void main()
{
    // fragColor = mix(texture(tex1, tcoord), texture(tex2, tcoord));
    fragColor = texture(tex1, tcoord);
}