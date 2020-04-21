#include "Mesh.h"

Mesh::Mesh() {
	load_buffers();
}

Mesh::Mesh(
	const std::vector<Texture>& textures,
	const std::vector<glm::vec3>& vertices,
	const std::vector<glm::vec2>& uvs,
	const std::vector<glm::vec3>& normals,
	const std::vector<unsigned short>& indices,
	const Transform& transform
) :
	textures		( textures  ),
	vertices		( vertices  ),
	uvs				( uvs       ),
	normals			( normals   ),
	indices			( indices   ),
	transform		( transform )
{
	load_buffers();
}

void Mesh::load_buffers() {
	glCreateVertexArrays(1, &_vao);

	glCreateBuffers(1, &_vertex_buffer);
	if (vertices.size() > 0) {
		glBindBuffer(GL_ARRAY_BUFFER, _vertex_buffer);
		glNamedBufferStorage(_vertex_buffer, sizeof(glm::vec3) * vertices.size(), &vertices[0], 0);
	}

	glCreateBuffers(1, &_uv_buffer);
	if (uvs.size() > 0) {
		glBindBuffer(GL_ARRAY_BUFFER, _uv_buffer);
		glNamedBufferStorage(_uv_buffer, sizeof(glm::vec2) * uvs.size(), &uvs[0], 0);
	}

	glCreateBuffers(1, &_normal_buffer);
	if (normals.size() > 0) {
		glBindBuffer(GL_ARRAY_BUFFER, _normal_buffer);
		glNamedBufferStorage(_normal_buffer, sizeof(glm::vec3) * normals.size(), &normals[0], 0);
	}

	glCreateBuffers(1, &_indices_buffer);
	if (indices.size() > 0) {
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _indices_buffer);
		glNamedBufferStorage(_indices_buffer, sizeof(unsigned short) * indices.size(), &indices[0], 0);
	}
}

void Mesh::draw(const GLuint program) {
	glBindVertexArray(_vao);
	glUseProgram(program);

	glBindBuffer(GL_ARRAY_BUFFER, _vertex_buffer);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

	glBindBuffer(GL_ARRAY_BUFFER, _uv_buffer);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

	glBindBuffer(GL_ARRAY_BUFFER, _normal_buffer);
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

	glUniformMatrix4fv(glGetUniformLocation(program, "model"), 1, GL_FALSE, &transform.get_model()[0][0]);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _indices_buffer);

	for (unsigned int i = 0; i < textures.size(); ++i) {
		glActiveTexture(GL_TEXTURE0 + i);

		glBindTexture(GL_TEXTURE_2D, textures[i].id);
	}

	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_SHORT, (void*)0);
}