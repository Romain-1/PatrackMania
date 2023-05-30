#version 440

in vec3 f_color;
in vec2 uv;

out vec4 FragColor;
uniform sampler2D TextureSampler;

void main()
{
	FragColor = vec4(f_color * texture(TextureSampler, uv).rgb, 1.0);
}