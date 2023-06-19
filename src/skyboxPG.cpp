#include "simplegl.h"
#include "skyboxPG.hpp"
#include "compiler_settings.h"

#ifdef SGL_DEBUG
 #define SGL_SKYBOXPG_DEBUG
#endif
#ifdef SGL_SKYBOXPG_DEBUG 
 #define D(x) std::cout << "[SkyboxPG] " << x ;
 #define D_(x) x
 #define D_SPACER "-- skyboxPG.cpp -------------------------------------------------\n"
 #define D_SPACER_END "----------------------------------------------------------------\n"
#else 
 #define D(x)
 #define D_(x)
 #define D_SPACER ""
 #define D_SPACER_END ""
#endif

SkyboxPG::SkyboxPG(std::string vs_file, std::string fs_file)
	: Program(vs_file, fs_file)
{
	D(__PRETTY_FUNCTION__ << "\n")
	this->getLocations();
}

SkyboxPG::~SkyboxPG() {
	D(__PRETTY_FUNCTION__ << "\n")
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
	//D("render Skybox" << "\n")
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
			D("dynamic_cast<Obj3d*> failed on Object : " << o << std::endl)
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
	D(__PRETTY_FUNCTION__ << " : " << this->_program << "\n")
	this->_mat4_vp = this->getSlot("VP", true);
	this->_cubemap = this->getSlot("cubemap", true);

	this->_vertex_position_data = this->getSlot("vertex_position_data", false);
}

//accessors
GLint	SkyboxPG::getSlotCubemap() const { return (this->_cubemap); }