#include "simplegl.h"
#include "obj3dPG.hpp"

Obj3dPG::Obj3dPG(std::string vs_file, std::string fs_file) : \
Program(vs_file, fs_file) {
	cout << "_ Obj3dProgram cons" << endl;

	//this->getLocations();

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

void	Obj3dPG::render(Object& object, Math::Matrix4 PVmatrix) const {
	Obj3d* obj = dynamic_cast<Obj3d*>(&object);
	if (!obj) {
		std::cout << "dynamic_cast<Obj3d*> failed on Object : " << obj << std::endl;
		exit(22);
	}
	const Math::Vector3 &	color = obj->getColorShader();
	Obj3dBP& bp = obj->getBlueprint();
	// Math::Matrix4&	modelMatrix = obj->getParent() ? obj->getWorldMatrix() : obj->local.getMatrix();
	Math::Matrix4& modelMatrix = obj->getWorldMatrix();

	// cout << "rendering " << bp.getName() << " #" << obj->getId() << " vao:" << bp.getVao() << endl;
	// cout << "*\tpolygons: " << bp.getFaceAmount() << endl;

	PVmatrix.mult(modelMatrix);
	PVmatrix.setOrder(COLUMN_MAJOR);

	//can be done once for all obj3d
	// glUseProgram(this->_program);

	glUniformMatrix4fv(this->_mat4_mvp, 1, GL_FALSE, PVmatrix.getData());
	glUniform1i(this->_dismod, 1);// 1 = display plain_color, 0 = vertex_color
	//plain_color should not be used, check shader
	glUniform3f(this->_plain_color, color.x, color.y, color.z);

	glBindVertexArray(bp.getVao());
	if (obj->displayTexture && obj->getTexture() != nullptr) {
		glUniform1f(this->_tex_coef, 1.0f);
		glBindTexture(GL_TEXTURE_2D, obj->getTexture()->getId());
		glBindBuffer(GL_ARRAY_BUFFER, bp.getVboTexture());
		glVertexAttribPointer(this->_vertex_UV_data, 2, GL_FLOAT, GL_FALSE, 0, NULL);
	}
	else
		glUniform1f(this->_tex_coef, 0.0f);
	glPolygonMode(GL_FRONT_AND_BACK, obj->getPolygonMode());
	glDrawArrays(GL_TRIANGLES, 0, bp.getFaceAmount() * 3);

	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void	Obj3dPG::renderUniqueId(Obj3d& obj, Math::Matrix4 PVmatrix) const {
	Obj3dBP &		bp = obj.getBlueprint();
	Math::Matrix4&	modelMatrix = obj.getWorldMatrix();

	// cout << "rendering " << bp.getName() << " #" << obj.getId() << " vao:" << bp.getVao() << endl;
	// cout << "*\tpolygons: " << bp.getFaceAmount() << endl;
	
	PVmatrix.mult(modelMatrix);
	PVmatrix.setOrder(COLUMN_MAJOR);
	
	glUniformMatrix4fv(this->_mat4_mvp, 1, GL_FALSE, PVmatrix.getData());
	glUniform1i(this->_dismod, 1);// 1 = display plain_color, 0 = vertex_color
	//plain_color should not be used, check shader
	unsigned int id = obj.getId();
	uint8_t	rgb[3];
	Misc::intToRGB(id, &(*rgb));
	glUniform3f(this->_plain_color, float(rgb[0])/255.0f, float(rgb[1])/255.0f, float(rgb[2])/255.0f);
	//should store adress of object directly, if we can store RGBA with the shader

	glBindVertexArray(bp.getVao());
	glUniform1f(this->_tex_coef, 0.0f);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glDrawArrays(GL_TRIANGLES, 0, bp.getFaceAmount() * 3);

	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void	Obj3dPG::renderObjects(list<Object*>& list, Cam& cam, bool force_draw) {
	// cout << "render all Obj3d" << endl;
	if (list.empty())
		return;
	//assuming all Obj3d have the same program
	glUseProgram(this->_program);//used once for all obj3d
	Math::Matrix4	viewProMatrix(cam.getProjectionMatrix());
	Math::Matrix4	viewMatrix = cam.getViewMatrix();
	viewProMatrix.mult(viewMatrix);// do it in shader ? NO cauz shader will do it for every vertix

	unsigned int	counterForward = 0;
	unsigned int	counterFrustum = 0;
	Math::Vector3	center;
	Math::Vector3	dim;
	Math::Vector3	scale;
	Math::Vector3	oldcolor;
	Math::Vector3	color;
	Math::Matrix4	worldmatrix;

	for (Object* o : list) {
		Obj3d* object = dynamic_cast<Obj3d*>(o);
		if (!object) {
			std::cout << "dynamic_cast<Obj3d*> failed on Object : " << o << std::endl;
			exit(22);
		}
		else {
			object->update();
			if (object->getParent()) {
				worldmatrix = object->getWorldMatrix();
				worldmatrix.setOrder(COLUMN_MAJOR);
				float* m = worldmatrix.getData();
				center = Math::Vector3(m[12], m[13], m[14]);//world pos and not real center
			} else {
				center = object->local.getPos();//== world pos
			}
			scale = object->local.getScale();//what if parent?
			dim = object->getBlueprint().getDimensions();//what if parent?
			dim.x *= scale.x;
			dim.y *= scale.y;
			dim.z *= scale.z;
			center.add(dim.x / 2.0f, dim.y / 2.0f, dim.z / 2.0f);//what about the rotation

			/*
				get the center of the object BP, apply the rotation, add up to the world pos
				for the voxel engine, it doesnt matter since the voxels dont have parent or rotation
			*/

			bool draw = true;
			oldcolor = object->getColor();
			color = oldcolor;
			if (cam.isInFrustum(center, viewProMatrix)) {
				//color = Math::Vector3(40, 200, 200);//cyan
				counterFrustum++;
			} else if (cam.local.forwardDistance(center) > 0) {
				color = Math::Vector3(200, 200, 40);//yellow
				counterForward++;
				draw = false;
			}
			else {
				draw = false;
			}

			if (object->getPolygonMode() == GL_LINE) {
				color = oldcolor;
			}

			if (force_draw || draw) {
				object->setColor(color.x, color.y, color.z);
				object->render(viewProMatrix);
				object->setColor(oldcolor.x, oldcolor.y, oldcolor.z);
			}
		}

	}

	//std::cout << "fustrum objects: " << counterFrustum << std::endl;
	//std::cout << "forward objects: " << counterForward << "\t(not in fustrum)" << std::endl;
	//std::cout << "total objects: " << list.size() << std::endl;
	//std::cout << std::endl;
	for (Object* o : list) {//to do AFTER all objects are rendered
		Obj3d* object = dynamic_cast<Obj3d*>(o);
		if (!object) {
			std::cout << "dynamic_cast<Obj3d*> failed on Object : " << o << std::endl;
			Misc::breakExit(22);
		} else {
			object->local._matrixChanged = false;
			object->_worldMatrixChanged = false;
		}
	}
}

void	Obj3dPG::getLocations() {
	/* refacto: envoyer la fonction dans la classe Program:
		getLocations(stringList Uniform, stringList Attribute)
		stock dans un map string:int (varname:slot)

		pros:
			comportement uniforme entre obj3d/skybox/autre
		cons:
			plus lent a l'utilisation ?
	*/

	/*
		true	glGetUniformLocation
		false	glGetAttribLocation
	*/

	cout << "Getting slots for obj3d program " << this->_program << endl;
	this->_mat4_mvp = this->getSlot("MVP", true);
	this->_dismod = this->getSlot("dismod", true);
	this->_plain_color = this->getSlot("plain_color", true);
	this->_tex_coef = this->getSlot("tex_coef", true);

	this->_vertex_position_data = this->getSlot("vertex_position_data", false);
	this->_vertex_color_data = this->getSlot("vertex_color_data", false);
	this->_vertex_UV_data = this->getSlot("vertex_UV_data", false);
}
