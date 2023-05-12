#include "MeshRenderer.hpp"
#include <GL/gl3w.h>
#include <glm/ext/matrix_transform.hpp>

#include <tiny_gltf.h>
#include <glm/gtx/string_cast.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Console.hpp"

MeshRenderer::MeshRenderer(
	const tinygltf::Model& model,
	const tinygltf::Node& node,
	int drawMode
) : m_drawMode(drawMode), useTexture(false)
{
	auto& mesh = model.meshes[node.mesh];
	auto primitive = mesh.primitives[0];

	glGenVertexArrays(1, &m_handler.vao);
	glBindVertexArray(m_handler.vao);

	const tinygltf::Accessor& accessorIndices = model.accessors[primitive.indices];
	const tinygltf::BufferView& bufferViewIndices = model.bufferViews[accessorIndices.bufferView];
	const tinygltf::Buffer& bufferIndices = model.buffers[bufferViewIndices.buffer];
	const unsigned int* indices = reinterpret_cast<const unsigned int*>(&bufferIndices.data[bufferViewIndices.byteOffset + accessorIndices.byteOffset]);

	const tinygltf::Accessor& accessorPosition = model.accessors[primitive.attributes["POSITION"]];
	const tinygltf::BufferView& bufferViewPosition = model.bufferViews[accessorPosition.bufferView];
	const tinygltf::Buffer& bufferPosition = model.buffers[bufferViewPosition.buffer];
	const float* positions = reinterpret_cast<const float*>(&bufferPosition.data[bufferViewPosition.byteOffset + accessorPosition.byteOffset]);

	const tinygltf::Accessor& accessorNormal = model.accessors[primitive.attributes["NORMAL"]];
	const tinygltf::BufferView& bufferViewNormal = model.bufferViews[accessorNormal.bufferView];
	const tinygltf::Buffer& bufferNormal = model.buffers[bufferViewNormal.buffer];
	const float* normals = reinterpret_cast<const float*>(&bufferNormal.data[bufferViewNormal.byteOffset + accessorNormal.byteOffset]);

	glGenBuffers(1, &m_handler.ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_handler.ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * accessorIndices.count, indices, GL_STATIC_DRAW);

	glGenBuffers(1, &m_handler.vbo[0]);
	glBindBuffer(GL_ARRAY_BUFFER, m_handler.vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 3 * accessorPosition.count, positions, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	glGenBuffers(1, &m_handler.vbo[1]);
	glBindBuffer(GL_ARRAY_BUFFER, m_handler.vbo[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 3 * accessorNormal.count, normals, GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);

	std::vector<float> tmp;
	tmp.resize(accessorPosition.count * 2);
	const float* textures = tmp.data();
	if (primitive.attributes.find("TEXCOORD_0") != primitive.attributes.end()) {
		const tinygltf::Accessor& accessorTexture = model.accessors[primitive.attributes["TEXCOORD_0"]];
		const tinygltf::BufferView& bufferViewTexture = model.bufferViews[accessorTexture.bufferView];
		const tinygltf::Buffer& bufferTexture = model.buffers[bufferViewTexture.buffer];
		textures = reinterpret_cast<const float*>(&bufferTexture.data[bufferViewTexture.byteOffset + accessorTexture.byteOffset]);
	}

	glGenBuffers(1, &m_handler.vbo[2]);
	glBindBuffer(GL_ARRAY_BUFFER, m_handler.vbo[2]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 2 * accessorPosition.count, textures, GL_STATIC_DRAW);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(2);


	if (primitive.material < 0) return;

	auto & m = model.materials[primitive.material];
	if (m.pbrMetallicRoughness.baseColorTexture.index != -1) {
		useTexture = true;
		auto& t = model.textures[m.pbrMetallicRoughness.baseColorTexture.index];
		auto& img = model.images[t.source];
		glGenTextures(1, &m_handler.textureId);
		glBindTexture(GL_TEXTURE_2D, m_handler.textureId);
		////TODO mutliple textures
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, img.width, img.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, img.image.data());
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	}

	glBindVertexArray(0);
}

void MeshRenderer::Draw(ShaderProgram* shaderProgram)
{
	glBindTexture(GL_TEXTURE_2D, m_handler.textureId);
	glUniform1iv(shaderProgram->uniform("UseTexture"), 1, &useTexture);
	glBindVertexArray(m_handler.vao);
	int size;
	glGetBufferParameteriv(GL_ELEMENT_ARRAY_BUFFER, GL_BUFFER_SIZE, &size);
	glDrawElements(m_drawMode, size / sizeof(unsigned int), GL_UNSIGNED_INT, 0);
}

// TODO get rid of that
void MeshRenderer::ToggleTexture(bool state)
{
	useTexture = state;
}