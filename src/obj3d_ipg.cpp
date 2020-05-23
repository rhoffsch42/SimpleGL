#include "obj3d_ipg.hpp"
#include <vector>

Obj3dIPG::Obj3dIPG(std::string vertexShader, std::string fragmentShader, bool init_locations)
	: Obj3dPG(vertexShader, fragmentShader, false)
{
	cout << "_ " << __PRETTY_FUNCTION__ << endl;
	if (init_locations)
		this->Obj3dIPG::getLocations();//explicit equivalent of: this->getLocations();
	glGenBuffers(1, &this->_vboMatrix);
	this->_vboSize = 0;
}
Obj3dIPG::~Obj3dIPG() {}

void	Obj3dIPG::render(Object& object, Math::Matrix4 VPmatrix) const { //HAS to be cpy constructor! (1 PV*M per obj3d)
	(void)object;
	(void)VPmatrix;
}
void	Obj3dIPG::renderObjects(list<Object*>& list, Cam& cam, uint8_t flags) {
	/*
		// https://learnopengl.com/Advanced-OpenGL/Instancing

		convert list<Object*> (in fact Obj3d*) matrices into vector<float>
		all matrices must be COLUMN_MAJOR
		all objects must come from the same BP
	*/
	if (list.empty())
		return;
	Math::Matrix4	VPmatrix(cam.getProjectionMatrix());
	Math::Matrix4	Vmatrix = cam.getViewMatrix();
	VPmatrix.mult(Vmatrix);// do it in shader ? NO cauz shader will do it for every vertice

	// this part needs threads?
	int		instances_amount = list.size();
	int		index = 0;
	int		array_size = instances_amount * 16;//16 float for a matrix
	float*	mvp_concatened = new float[array_size];

	for (auto o : list) {// build all MVP (cam is moving so we have to do this each frame)
		Obj3d* ptr = dynamic_cast<Obj3d*>(o);
		if (ptr) {
			ptr->update();
			//check for frustum
			//if (flags & PG_FRUSTUM_CULLING) {}
			Math::Matrix4	MVPmatrix(VPmatrix);
			MVPmatrix.mult(ptr->getWorldMatrix());
			MVPmatrix.setOrder(COLUMN_MAJOR);
			memcpy(mvp_concatened + index * 16, MVPmatrix.getData(), 16 * sizeof(float));
		} else {
			std::cout << "dynamic_cast<Obj3d*>(o) failed: " << o << std::endl;
			Misc::breakExit(12);
		}
		index++;
	}
	//end threads

	Obj3d* obj = dynamic_cast<Obj3d*>(list.front());
	if (!obj) {
		std::cout << "dynamic_cast<Obj3d*>(list.front()) failed: " << list.front() << std::endl;
		Misc::breakExit(12);
	}
	Obj3dBP& bp = obj->getBlueprint();
	const Math::Vector3& color = obj->getColorShader();//will be used for every obj3d!
	glBindVertexArray(bp.getVao());

	glBindBuffer(GL_ARRAY_BUFFER, this->_vboMatrix);
	if (this->_vboSize >= instances_amount) {//use the current vbo as he is big enough
		glBufferSubData(GL_ARRAY_BUFFER, 0, array_size * sizeof(float), (void*)mvp_concatened);
	}
	else {// recreate a new vbo with the new size
		this->_vboSize = instances_amount;
		glBufferData(GL_ARRAY_BUFFER, array_size * sizeof(float), (void*)mvp_concatened, GL_DYNAMIC_DRAW);// GL_DYNAMIC_DRAW GL_STATIC_DRAW
	}
	delete[] mvp_concatened;

	std::size_t size4float = 4 * sizeof(float);
	for (int i = 0; i < 4; i++) {//could be done once?
		glEnableVertexAttribArray(this->_mat4_mvp + i);
		glVertexAttribPointer(this->_mat4_mvp + i, 4, GL_FLOAT, GL_FALSE, 4 * size4float, (const void*)(i * size4float));
		glVertexAttribDivisor(this->_mat4_mvp + i, 1);
	}

	glUseProgram(this->_program);//used once for all obj3d
	glUniform1i(this->_dismod, 0);// 1 = display plain_color, 0 = vertex_color (.mtl)
	glUniform3f(this->_plain_color, color.x, color.y, color.z);
	if (obj->displayTexture && obj->getTexture() != nullptr) {
		glUniform1f(this->_tex_coef, 1.0f);
		glBindTexture(GL_TEXTURE_2D, obj->getTexture()->getId());
	} else
		glUniform1f(this->_tex_coef, 0.0f);

	glPolygonMode(GL_FRONT_AND_BACK, obj->getPolygonMode());
	int	vertices_amount = bp.getPolygonAmount() * 3;
	if (bp.getDataMode() == BP_LINEAR)
		glDrawArraysInstanced(GL_TRIANGLES, 0, vertices_amount, instances_amount);
	else // should be BP_INDICES
		glDrawElementsInstanced(GL_TRIANGLES, vertices_amount, GL_UNSIGNED_INT, 0, instances_amount);
	/*
		++: when all is optimised build an obj3d with the vertex and indices and textures corresponding to the entire chunk! Modify it when adding or removing a cube.
		to make 1 draw of 1 obj3d
	*/

	//end
	for (Object* object : list) {//to do AFTER all objects are rendered
		object->local._matrixChanged = false;
		object->_worldMatrixChanged = false;
	}

	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);
}

void	Obj3dIPG::getLocations() {
	/*
		true	glGetUniformLocation
		false	glGetAttribLocation
	*/

	cout << "_ " << __PRETTY_FUNCTION__ << " : " << this->_program << endl;
	this->_dismod = this->getSlot("dismod", true);
	this->_plain_color = this->getSlot("plain_color", true);
	this->_tex_coef = this->getSlot("tex_coef", true);

	this->_vertex_position_data = this->getSlot("vertex_position_data", false);
	this->_vertex_color_data = this->getSlot("vertex_color_data", false);
	this->_vertex_UV_data = this->getSlot("vertex_UV_data", false);
	this->_mat4_mvp = this->getSlot("MVP", false);
}