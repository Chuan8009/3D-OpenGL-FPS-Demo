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
	_textures		( textures  ),
	_vertices		( vertices  ),
	_uvs			( uvs       ),
	_normals		( normals   ),
	_indices		( indices   ),
	_transform		( transform )
{
	load_buffers();
}

void Mesh::load_buffers() {
	glCreateVertexArrays(1, &_vao);

	glCreateBuffers(1, &_vertex_buffer);
	if (_vertices.size() > 0) {
		glBindBuffer(GL_ARRAY_BUFFER, _vertex_buffer);
		glNamedBufferStorage(_vertex_buffer, sizeof(glm::vec3) * _vertices.size(), &_vertices[0], 0);
	}

	glCreateBuffers(1, &_uv_buffer);
	if (_uvs.size() > 0) {
		glBindBuffer(GL_ARRAY_BUFFER, _uv_buffer);
		glNamedBufferStorage(_uv_buffer, sizeof(glm::vec2) * _uvs.size(), &_uvs[0], 0);
	}

	glCreateBuffers(1, &_normal_buffer);
	if (_normals.size() > 0) {
		glBindBuffer(GL_ARRAY_BUFFER, _normal_buffer);
		glNamedBufferStorage(_normal_buffer, sizeof(glm::vec3) * _normals.size(), &_normals[0], 0);
	}

	glCreateBuffers(1, &_indices_buffer);
	if (_indices.size() > 0) {
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _indices_buffer);
		glNamedBufferStorage(_indices_buffer, sizeof(unsigned short) * _indices.size(), &_indices[0], 0);
	}
}

void Mesh::draw(const GLuint program, Transform& transform) {
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

	for (unsigned int i = 0; i < _textures.size(); ++i) {
		glActiveTexture(GL_TEXTURE0 + i);

		glBindTexture(GL_TEXTURE_2D, _textures[i].id);
	}

	glDrawElements(GL_TRIANGLES, _indices.size(), GL_UNSIGNED_SHORT, (void*)0);
}

float two_decimal(float num) {
	num = (int)(num * 100.0f + .5f);
	return num / 100;
}

void Mesh::make_bounding_box() {
	_bounding_box.min = _vertices[0];
	_bounding_box.max = _vertices[0];

	for(auto v : _vertices) {
		if (v.x < _bounding_box.min.x)	_bounding_box.min.x = v.x;
		if (v.y < _bounding_box.min.y)	_bounding_box.min.y = v.y;
		if (v.z < _bounding_box.min.z)	_bounding_box.min.z = v.z;
		if (v.x > _bounding_box.max.x)	_bounding_box.max.x = v.x;
		if (v.y > _bounding_box.max.y)	_bounding_box.max.y = v.y;
		if (v.z > _bounding_box.max.z)	_bounding_box.max.z = v.z;
	}

	_bounding_box.min.x = two_decimal(_bounding_box.min.x);
	_bounding_box.min.y = two_decimal(_bounding_box.min.y);
	_bounding_box.min.z = two_decimal(_bounding_box.min.z);
	_bounding_box.max.x = two_decimal(_bounding_box.max.x);
	_bounding_box.max.y = two_decimal(_bounding_box.max.y);
	_bounding_box.max.z = two_decimal(_bounding_box.max.z);
}