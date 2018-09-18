#include "simplegl.h"
#include "skyboxPG.hpp"

SkyboxPG::SkyboxPG(std::string vs_file, std::string fs_file) : Program(vs_file, fs_file) {
	cout << "_ SkyboxPG cons" << endl;

	this->getLocations();
}

SkyboxPG::~SkyboxPG() {
	cout << "_ SkyboxPG des" << endl;
}

void	SkyboxPG::render(Skybox& skybox, Math::Matrix4& VPmatrix) {
	glUseProgram(this->_program);

	VPmatrix.setOrder(COLUMN_MAJOR);
	glUniformMatrix4fv(this->_mat4_vp, 1, GL_FALSE, VPmatrix.getData());
	glUniform1i(this->_cubemap, 0);//a quoi ca sert ?

	glBindTexture(GL_TEXTURE_CUBE_MAP, skybox.getCubemapId());
	glBindVertexArray(skybox.getVao());
	glBindBuffer(GL_ARRAY_BUFFER, skybox.getVboVertex());
	glVertexAttribPointer(this->_vertex_position_data, 3, GL_FLOAT, GL_FALSE, 0, NULL);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glDrawArrays(GL_TRIANGLES, 0, 12 * 3);

	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}

void	SkyboxPG::getLocations() {
	this->_mat4_vp = this->getSlot("VP", glGetUniformLocation);
	this->_cubemap = this->getSlot("cubemap", glGetUniformLocation);

	this->_vertex_position_data = this->getSlot("vertex_position_data", glGetAttribLocation);
}

//accessors
GLint	SkyboxPG::getSlotCubemap() const { return (this->_cubemap); }