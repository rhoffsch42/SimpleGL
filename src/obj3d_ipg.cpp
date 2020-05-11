#include "obj3d_ipg.hpp"
#include <vector>

Obj3dIPG::Obj3dIPG(std::string vertexShader, std::string fragmentShader) : Obj3dPG(vertexShader, fragmentShader) {
	//this->getLocations();
}
Obj3dIPG::~Obj3dIPG() {}

void	Obj3dIPG::render(Object& object, Math::Matrix4 VPmatrix) const { //HAS to be cpy constructor! (1 PV*M per obj3d)
	(void)object;
	(void)VPmatrix;
}
void	Obj3dIPG::renderObjects(list<Object*>& list, Cam& cam, bool force_draw) {
	/*
			convert list<Object*> (in fact Obj3d*) matrices into vector<float>
			all matrices must be COLUMN_MAJOR
			all objects must come from the same BP
	*/
	if (list.empty())
		return;
	Math::Matrix4	VPmatrix(cam.getProjectionMatrix());
	Math::Matrix4	Vmatrix = cam.getViewMatrix();
	VPmatrix.mult(Vmatrix);// do it in shader ? NO cauz shader will do it for every vertice

	std::vector<float>	mvpConcatened;
	for (auto o : list) {
		Obj3d* ptr = dynamic_cast<Obj3d*>(o);
		if (ptr) {
			ptr->update();
			//check for frustum
			Math::Matrix4	Mmatrix = ptr->getWorldMatrix();
			Math::Matrix4	MVPmatrix(VPmatrix);
			MVPmatrix.mult(Mmatrix);
			MVPmatrix.setOrder(COLUMN_MAJOR);
			float* matrix = MVPmatrix.getData();
			for (size_t i = 0; i < 16; i++) {
				mvpConcatened.push_back(matrix[i]);
			}
		} else {
			std::cout << "dynamic_cast<Obj3d*>(o) failed: " << o << std::endl;
			Misc::breakExit(12);
		}
	}

	Obj3d* obj = dynamic_cast<Obj3d*>(list.front());
	if (!obj) {
		std::cout << "dynamic_cast<Obj3d*>(list.front()) failed: " << list.front() << std::endl;
		Misc::breakExit(12);
	}
	Obj3dBP& bp = obj->getBlueprint();
	const Math::Vector3& color = obj->getColorShader();//will be used for every obj3d!
	GLint	vao = bp.getVao();
	glBindVertexArray(vao);

	// https://learnopengl.com/Advanced-OpenGL/Instancing
	GLuint	bufferMVPs;
	glGenBuffers(1, &bufferMVPs);
	glBindBuffer(GL_ARRAY_BUFFER, bufferMVPs);
	//for (size_t i = 0; i < 5; i++) {
	//	std::cout << data[i] << std::endl;
	//}
	glBufferData(GL_ARRAY_BUFFER, list.size() * sizeof(float) * 16, (void*)mvpConcatened.data(), GL_STATIC_DRAW);

	// vertex attributes
	std::size_t size4float = 4 * sizeof(float);
	for (int i = 0; i < 4; i++) {
		glEnableVertexAttribArray(this->_mat4_mvp + i);
		glVertexAttribPointer(this->_mat4_mvp + i, 4, GL_FLOAT, GL_FALSE, 4 * size4float, (const void*)(i * size4float));
		glVertexAttribDivisor(this->_mat4_mvp + i, 1);
	}

	glUseProgram(this->_program);//used once for all obj3d
	glUniform1i(this->_dismod, 1);// 1 = display plain_color, 0 = vertex_color
	//plain_color should not be used, check shader
	glUniform3f(this->_plain_color, color.x, color.y, color.z);
	if (obj->displayTexture && obj->getTexture() != nullptr) {
		glUniform1f(this->_tex_coef, 1.0f);
		glBindTexture(GL_TEXTURE_2D, obj->getTexture()->getId());
		glBindBuffer(GL_ARRAY_BUFFER, bp.getVboTexture());
		glVertexAttribPointer(this->_vertex_UV_data, 2, GL_FLOAT, GL_FALSE, 0, NULL);
	} else 
		glUniform1f(this->_tex_coef, 0.0f);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	int	vertices_amount = bp.getFaceAmount() * 3;
	int instances_amount = list.size();
	glDrawArraysInstanced(GL_TRIANGLES, 0, vertices_amount, instances_amount);
	//glDrawElementsInstanced(GL_TRIANGLES, indices_amount, GL_UNSIGNED_INT, 0, instances_amount);

	/*
		todo:
		glDrawElements use indices, to use it we need a BP that has a vbo with indices and a vbo with original vertex
		set a tag in the BP: BP_INDICES (for Elements) or BP_VERTEX_ARRAY (for Arrays)

		++: when all is optimised build an obj3d with the vertex and indices and textures corresponding to the entire chunk! Modify it when adding or removing a cube.
		to make 1 draw of 1 obj3d
	*/

	//end
	for (Object* o : list) {//to do AFTER all objects are rendered
		Obj3d* object = dynamic_cast<Obj3d*>(o);
		if (!object) {
			std::cout << "dynamic_cast<Obj3d*> failed on Object : " << o << std::endl;
			Misc::breakExit(22);
		}
		else {
			obj->local._matrixChanged = false;
			obj->_worldMatrixChanged = false;
		}
	}

	glDeleteBuffers(1, &bufferMVPs);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void	Obj3dIPG::getLocations() {
	/*
		true	glGetUniformLocation
		false	glGetAttribLocation
	*/

	cout << "Getting slots for obj3d_instanced program " << this->_program << endl;
	this->_dismod = this->getSlot("dismod", true);
	this->_plain_color = this->getSlot("plain_color", true);
	this->_tex_coef = this->getSlot("tex_coef", true);

	this->_vertex_position_data = this->getSlot("vertex_position_data", false);
	this->_vertex_color_data = this->getSlot("vertex_color_data", false);
	this->_vertex_UV_data = this->getSlot("vertex_UV_data", false);
	this->_mat4_mvp = this->getSlot("MVP", false);
}