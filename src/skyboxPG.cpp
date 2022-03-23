#include "skyboxPG.hpp"
#include "compiler_settings.h"

SkyboxPG::SkyboxPG(std::string vs_file, std::string fs_file)
	: Program(vs_file, fs_file)
{
	std::cout << "_ " << __PRETTY_FUNCTION__ << "\n";
	this->getLocations();
}

SkyboxPG::~SkyboxPG() {
	std::cout << "_ " << __PRETTY_FUNCTION__ << "\n";
}

void	SkyboxPG::render(Object& object, Math::Matrix4 VPmatrix) const {
	// glUseProgram(this->_program);
	Skybox*	skybox = dynamic_cast<Skybox*>(&object);
	VPmatrix.setOrder(COLUMN_MAJOR);
	glUniformMatrix4fv(this->_mat4_vp, 1, GL_FALSE, VPmatrix.getData());
	glUniform1i(this->_cubemap, 0);//a quoi ca sert ?

	glBindTexture(GL_TEXTURE_CUBE_MAP, skybox->getCubemapId());
	glBindVertexArray(skybox->getVao());
	glBindBuffer(GL_ARRAY_BUFFER, skybox->getVboVertex());
	glVertexAttribPointer(this->_vertex_position_data, 3, GL_FLOAT, GL_FALSE, 0, NULL);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glDrawArrays(GL_TRIANGLES, 0, 12 * 3);

	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}

void	SkyboxPG::renderObjects(std::list<Object*>& list, Cam& cam, unsigned int flags) {
	// cout << "render Skybox" << "\n";
	if (list.empty())
		return;
	//assuming all objects have the same program
	glUseProgram(this->_program);
	Math::Matrix4	viewProMatrix(cam.getProjectionMatrix());
	Math::Matrix4& viewMatrix = cam.getViewMatrix();
	viewProMatrix.mult(viewMatrix);
	for (Object* o : list) {
		Skybox* skybox = dynamic_cast<Skybox*>(o);
		if (!skybox) {
			std::cout << "dynamic_cast<Obj3d*> failed on Object : " << o << std::endl;
			Misc::breakExit(0);
		} else {
			skybox->render(viewProMatrix);
		}
	}
}


void	SkyboxPG::getLocations() {
	/*
		true	glGetUniformLocation
		false	glGetAttribLocation
	*/
	std::cout << "_ " << __PRETTY_FUNCTION__ << " : " << this->_program << "\n";
	this->_mat4_vp = this->getSlot("VP", true);
	this->_cubemap = this->getSlot("cubemap", true);

	this->_vertex_position_data = this->getSlot("vertex_position_data", false);
}

//accessors
GLint	SkyboxPG::getSlotCubemap() const { return (this->_cubemap); }