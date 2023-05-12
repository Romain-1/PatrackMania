#version 400

layout (location = 0) in vec3 VertexPosition;
layout (location = 1) in vec3 VertexNormal;
layout (location = 2) in vec2 VertexUV;

struct LightInfo {
	vec4 Position; // Light position in eye coords.
	vec3 La; // Ambient light intensity
	vec3 Ld; // Diffuse light intensity
	vec3 Ls; // Specular light intensity
};
uniform LightInfo Light;
struct MaterialInfo {
	vec3 Ka; // Ambient reflectivity
	vec3 Kd; // Diffuse reflectivity
	vec3 Ks; // Specular reflectivity
	float Shiness; // Specular shininess factor (phong exponent)
};
uniform MaterialInfo Material;

uniform mat4 ModelViewMatrix;
uniform mat3 NormalMatrix;
uniform mat4 MVP;

out vec3 f_color;
out vec2 uv;

void main(void)
{
	vec3 N = normalize(NormalMatrix * VertexNormal);
	vec4 P = ModelViewMatrix * vec4(VertexPosition, 1);
	vec3 L = normalize(vec3(Light.Position - P));

	vec3 R = reflect(-L, N);//normalize(2.0f * dot(-L, N) * N + L);
	vec3 V = vec3(normalize(-P));

	vec3 ambient = Light.La * Material.Ka;

	vec3 diffuse = Light.Ld * Material.Kd * max(dot(L, N), 0);

	vec3 specular = Light.Ls * Material.Ks * pow(max(dot(R, V), 0.0), Material.Shiness);

	f_color = (ambient + diffuse + specular);
	uv = VertexUV;
	//uv_color = texture(TextureSampler, VertexUV).rgb;

	gl_Position = MVP * vec4(VertexPosition, 1);
}