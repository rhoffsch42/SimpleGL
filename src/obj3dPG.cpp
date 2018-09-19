#include "simplegl.h"
#include "obj3dPG.hpp"

Obj3dPG::Obj3dPG(std::string vs_file, std::string fs_file) : \
Program(vs_file, fs_file) {
	cout << "_ Obj3dProgram cons" << endl;

	this->getLocations();

	cout << "_ Obj3dProgram cons end" << endl;
}

Obj3dPG::~Obj3dPG() {
	cout << "_ Obj3dProgram des" << endl;

}

void	Obj3dPG::linkBuffers(GLuint& vboVertex, GLuint& vboColor, GLuint& vboTexture) const {
	glBindBuffer(GL_ARRAY_BUFFER, vboVertex);
	glVertexAttribPointer(this->_vertex_position_data, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(this->_vertex_position_data);

	glBindBuffer(GL_ARRAY_BUFFER, vboColor);
	glVertexAttribPointer(this->_vertex_color_data, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(this->_vertex_color_data);

	glBindBuffer(GL_ARRAY_BUFFER, vboTexture);
	glVertexAttribPointer(this->_vertex_UV_data, 2, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(this->_vertex_UV_data);
}

void	Obj3dPG::render(Obj3d& obj, Math::Matrix4 PVmatrix) {
	Math::Vector3	color = obj.getColor();
	Obj3dBP&		bp = obj.getBlueprint();
	Math::Matrix4&	modelMatrix = obj.getModelMatrix();

	// cout << "rendering " << bp.getName() << " #" << obj.getId() << " vao:" << bp.getVao() << endl;
	// cout << "*\tpolygons: " << bp.getFaceAmount() << endl;
	
	PVmatrix.mult(modelMatrix);
	PVmatrix.setOrder(COLUMN_MAJOR);
	glUniformMatrix4fv(this->_mat4_mvp, 1, GL_FALSE, PVmatrix.getData());
	glUniform1i(this->_dismod, 0);// 1 = display plain_color, 0 = vertex_color
	glUniform3f(this->_plain_color, color.x, color.y, color.z);

	glBindVertexArray(bp.getVao());
	if (obj._displayTexture && obj.getTexture() != NULL) {
		glUniform1f(this->_tex_coef, 1.0f);
		glBindTexture(GL_TEXTURE_2D, obj.getTexture()->getId());
		glBindBuffer(GL_ARRAY_BUFFER, bp.getVboTexture());
		glVertexAttribPointer(this->_vertex_UV_data, 2, GL_FLOAT, GL_FALSE, 0, NULL);
	} else
		glUniform1f(this->_tex_coef, 0.0f);
	glPolygonMode(GL_FRONT_AND_BACK, obj.getPolygonMode());
	if (bp.getName() == "obj3d/lambo/Lamborginhi_Aventador_OBJ/Lamborghini_Aventador.obj") {//tmp
		int groups[] = {7268, 128, 714, 714, 714, 714, 122};
		int size = sizeof(groups) / 4;
		int ind = 0;
		for (int i = 0; i < size - 1; i++) {
			glDrawArrays(GL_TRIANGLES, ind, groups[i] * 3);
			ind += (groups[i] * 3);
		}
	}
	else
		glDrawArrays(GL_TRIANGLES, 0, bp.getFaceAmount() * 3);
	
	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);

}

void	Obj3dPG::getLocations(void) {
	/* refacto: envoyer la fonction dans la classe Program:
		getLocations(stringList Uniform, stringList Attribute)
		stock dans un map string:int (varname:slot)

		pros:
			comportement uniforme entre obj3d/skybox/autre
		cons:
			plus lent a l'utilisation ?
	*/
	cout << "Getting slots..." << endl;
	this->_mat4_mvp = this->getSlot("MVP", glGetUniformLocation);
	this->_dismod = this->getSlot("dismod", glGetUniformLocation);
	this->_plain_color = this->getSlot("plain_color", glGetUniformLocation);
	this->_tex_coef = this->getSlot("tex_coef", glGetUniformLocation);

	this->_vertex_position_data = this->getSlot("vertex_position_data", glGetAttribLocation);
	this->_vertex_color_data = this->getSlot("vertex_color_data", glGetAttribLocation);
	this->_vertex_UV_data = this->getSlot("vertex_UV_data", glGetAttribLocation);
}
