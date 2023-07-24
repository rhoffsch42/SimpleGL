#include "simplegl.h"
#include "obj3d_ipg.hpp"
#include "compiler_settings.h"
#include <vector>

#ifdef SGL_DEBUG
 #define SGL_OBJ3D_IPG_DEBUG
#endif
#ifdef SGL_OBJ3D_IPG_DEBUG 
 #define D(x) std::cout << "[Obj3dIPG] " << x ;
 #define D_(x) x
 #define D_SPACER "-- obj3d_ipg.cpp -------------------------------------------------\n"
 #define D_SPACER_END "----------------------------------------------------------------\n"
#else 
 #define D(x)
 #define D_(x)
 #define D_SPACER ""
 #define D_SPACER_END ""
#endif


Obj3dIPG::Obj3dIPG(std::string vertexShader, std::string fragmentShader, bool init_locations)
	: Obj3dPG(vertexShader, fragmentShader, false)
{
	D(__PRETTY_FUNCTION__ << std::endl)
	if (init_locations)
		this->Obj3dIPG::getLocations();//explicit equivalent of: this->getLocations();
	glGenBuffers(1, &this->_vboMatrix);
	this->_vboSize = 0;
}
Obj3dIPG::~Obj3dIPG() {
	glDeleteBuffers(1, &this->_vboMatrix);
}

void	Obj3dIPG::renderObject(Object& object, Math::Matrix4 VPmatrix) const { //HAS to be cpy constructor! (1 PV*M per obj3d)
	(void)object;
	(void)VPmatrix;
}
void	Obj3dIPG::renderAllObjects(std::vector<Object*>& objects, Cam& cam, unsigned int flags) {
	/*
		// https://learnopengl.com/Advanced-OpenGL/Instancing

		convert vector<Object*> (in fact Obj3d*) matrices into vector<float>
		all matrices must be COLUMN_MAJOR
		all objects must come from the same BP
	*/
	if (objects.empty())
		return;
	Math::Matrix4	VPmatrix(cam.getProjectionMatrix());
	Math::Matrix4	Vmatrix = cam.getViewMatrix();
	VPmatrix.mult(Vmatrix);// do it in shader ? NO cauz shader will do it for every vertice

	// this part needs threads?
	size_t	instances_amount = objects.size();
	int		index = 0;
	size_t	array_size = instances_amount * 16;//16 float for a matrix
	float*	mvp_concatened = new float[array_size];

	for (auto object : objects) {// build all MVP (cam is moving so we have to do this each frame)
		Obj3d* obj = dynamic_cast<Obj3d*>(object);
		if (obj) {
			obj->update();
			//check for frustum
			//if (flags & PG_FRUSTUM_CULLING) {}
			Math::Matrix4	MVPmatrix(VPmatrix);
			MVPmatrix.mult(obj->getWorldMatrix());
			MVPmatrix.setOrder(COLUMN_MAJOR);
			memcpy(mvp_concatened + index * 16, MVPmatrix.getData(), 16 * sizeof(float));
		}
		else {
			D("dynamic_cast<Obj3d*>(o) failed: " << object << std::endl)
			Misc::breakExit(12);
		}
		index++;
	}
	//end threads
	if (index != instances_amount) {// (or some obj have been culled)
		std::cout << "Missing data : " << index << " != " << instances_amount << "\n";
		Misc::breakExit(12);
	}

	Obj3d* obj = dynamic_cast<Obj3d*>(objects.front());
	if (!obj) {
		D("dynamic_cast<Obj3d*>(list.front()) failed: " << list.front() << std::endl)
		Misc::breakExit(12);
	}
	Obj3dBP* bp = obj->getBlueprint();
	const Math::Vector3& color = obj->getColorShader();//will be used for every obj3d!
	glBindVertexArray(bp->getVao());
	glUseProgram(this->_program);//used once for all obj3d

	glBindBuffer(GL_ARRAY_BUFFER, this->_vboMatrix);
	if (this->_vboSize >= instances_amount) {//use the current vbo as he is big enough
		glBufferSubData(GL_ARRAY_BUFFER, 0, array_size * sizeof(float), (void*)mvp_concatened);
	}
	else {// recreate a new vbo with the new size
		this->_vboSize = instances_amount;
		glBufferData(GL_ARRAY_BUFFER, array_size * sizeof(float), (void*)mvp_concatened, GL_DYNAMIC_DRAW);// GL_DYNAMIC_DRAW GL_STATIC_DRAW
	}
	delete[] mvp_concatened;

	int size4float = 4 * sizeof(float);
	for (int i = 0; i < 4; i++) {//could be done once?
		glEnableVertexAttribArray(this->_mat4_mvp + i);
		glVertexAttribPointer(this->_mat4_mvp + i, 4, GL_FLOAT, GL_FALSE, 4 * size4float, (const void*)(i * size4float));
		glVertexAttribDivisor(this->_mat4_mvp + i, 1);
	}

	glUniform1i(this->_dismod, 0);// 1 = display plain_color, 0 = vertex_color (.mtl)
	glUniform3f(this->_plain_color, color.x, color.y, color.z);
	if (obj->displayTexture && obj->getTexture() != nullptr) {
		glUniform1f(this->_tex_coef, 1.0f);
		glBindTexture(GL_TEXTURE_2D, obj->getTexture()->getId());
	}
	else { glUniform1f(this->_tex_coef, 0.0f); }

	glPolygonMode(GL_FRONT_AND_BACK, obj->getPolygonMode());
	int	vertices_amount = bp->getPolygonAmount() * 3;
	if (bp->getDataMode() == BP_LINEAR)
		glDrawArraysInstanced(GL_TRIANGLES, 0, vertices_amount, instances_amount);
	else { // should be BP_INDICES
		//unsigned int face = cam.speed;
		//glDrawElementsInstanced(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)(EBO_OFFSET(face)), instances_amount);
		glDrawElementsInstanced(GL_TRIANGLES, vertices_amount, GL_UNSIGNED_INT, 0, instances_amount);
	}

	for (Object* object : objects) {//to do AFTER all objects are rendered
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

	D(__PRETTY_FUNCTION__ << " : " << this->_program << std::endl)
	this->_dismod = this->getSlot("dismod", true);
	this->_plain_color = this->getSlot("plain_color", true);
	this->_tex_coef = this->getSlot("tex_coef", true);

	this->_vertex_position_data = this->getSlot("vertex_position_data", false);
	this->_vertex_color_data = this->getSlot("vertex_color_data", false);
	this->_vertex_UV_data = this->getSlot("vertex_UV_data", false);
	this->_mat4_mvp = this->getSlot("MVP", false);
}
