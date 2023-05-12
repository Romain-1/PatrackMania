#version 440

in vec3 f_color;
in vec2 uv;

out vec4 FragColor;
uniform sampler2D TextureSampler;
uniform bool UseTexture;

void main()
{
	if (UseTexture == true) {
		FragColor = vec4(f_color * texture(TextureSampler, uv).rgb, 1.0);
	} else {
		FragColor = vec4(f_color, 1.0);
	}
}