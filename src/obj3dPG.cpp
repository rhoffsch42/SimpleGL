#include "simplegl.h"
#include "obj3dPG.hpp"
#include "compiler_settings.h"

#ifdef SGL_DEBUG
 #define SGL_OBJ3DPG_DEBUG
#endif
#ifdef SGL_OBJ3DPG_DEBUG 
 #define D(x) std::cout << "[Obj3dPG] " << x ;
 #define D_(x) x
 #define D_SPACER "-- obj3dPG.cpp -------------------------------------------------\n"
 #define D_SPACER_END "----------------------------------------------------------------\n"
#else 
 #define D(x)
 #define D_(x)
 #define D_SPACER ""
 #define D_SPACER_END ""
#endif

Obj3dPG::Obj3dPG(std::string vs_file, std::string fs_file, bool init_locations)
	: Program(vs_file, fs_file)
{
	D(__PRETTY_FUNCTION__ << std::endl)
	if (init_locations)
		this->getLocations();
	D(__PRETTY_FUNCTION__ << " END" << std::endl)
}

Obj3dPG::~Obj3dPG() {
	D(__PRETTY_FUNCTION__ << std::endl)
}

/*
	a vao exists only in the context it was created in
*/
void	Obj3dPG::linkBuffers(const Obj3dBP& blueprint) const {
	/*
		glVertexAttribPointer 5th arg is stride size: 0 = tightly packed, so opengl will determine itself
	*/
	glBindVertexArray(blueprint.getVao());

	glBindBuffer(GL_ARRAY_BUFFER, blueprint.getVboVertex());
	glEnableVertexAttribArray(this->_vertex_position_data);
	glVertexAttribPointer(this->_vertex_position_data, 3, GL_FLOAT, GL_FALSE, sizeof(SimpleVertex), (void*)0);
	glEnableVertexAttribArray(this->_vertex_UV_data);
	glVertexAttribPointer(this->_vertex_UV_data, 2, GL_FLOAT, GL_FALSE, sizeof(SimpleVertex), (void*)offsetof(SimpleVertex, texCoord));
	glEnableVertexAttribArray(this->_vertex_color_data);
	glVertexAttribPointer(this->_vertex_color_data, 3, GL_FLOAT, GL_FALSE, sizeof(SimpleVertex), (void*)offsetof(SimpleVertex, color));

	glBindVertexArray(0);
}
void	Obj3dPG::linkBuffersToVao(const Obj3dBP& blueprint, GLuint vao) const {
	/*
		glVertexAttribPointer 5th arg is stride size: 0 = tightly packed, so opengl will determine itself
	*/
	glBindVertexArray(vao);

	glBindBuffer(GL_ARRAY_BUFFER, blueprint.getVboVertex());
	glEnableVertexAttribArray(this->_vertex_position_data);
	glVertexAttribPointer(this->_vertex_position_data, 3, GL_FLOAT, GL_FALSE, sizeof(SimpleVertex), (void*)0);
	glEnableVertexAttribArray(this->_vertex_UV_data);
	glVertexAttribPointer(this->_vertex_UV_data, 2, GL_FLOAT, GL_FALSE, sizeof(SimpleVertex), (void*)offsetof(SimpleVertex, texCoord));
	glEnableVertexAttribArray(this->_vertex_color_data);
	glVertexAttribPointer(this->_vertex_color_data, 3, GL_FLOAT, GL_FALSE, sizeof(SimpleVertex), (void*)offsetof(SimpleVertex, color));

	glBindVertexArray(0);
}

void	Obj3dPG::render(Object& object, Math::Matrix4 PVmatrix) const {
	Obj3d* obj = dynamic_cast<Obj3d*>(&object);
	if (!obj) {
		D("dynamic_cast<Obj3d*> failed on Object : " << obj << std::endl)
		// Misc::breakExit(22);
		// this can happen when an object is being nulled to be replaced by another one, block or continue?
		return;
	}
	const Math::Vector3& color = obj->getColorShader();
	Obj3dBP* bp = obj->getBlueprint();
	// Math::Matrix4&	modelMatrix = obj->getParent() ? obj->getWorldMatrix() : obj->local.getMatrix();
	Math::Matrix4& modelMatrix = obj->getWorldMatrix();

	//D("rendering " << bp.getName() << " #" << obj->getId() << " vao:" << bp.getVao() << std::endl)
	//D("*\tpolygons: " << bp.getPolygonAmount() << std::endl)

	PVmatrix.mult(modelMatrix);
	PVmatrix.setOrder(COLUMN_MAJOR);

	//can be done once for all obj3d
	glUseProgram(this->_program);

	glUniformMatrix4fv(this->_mat4_mvp, 1, GL_FALSE, PVmatrix.getData());
	glUniform1i(this->_dismod, 0);// 1 = display plain_color, 0 = vertex_color (.mtl)
	glUniform3f(this->_plain_color, color.x, color.y, color.z);

	//D("rendering BP " << &bp << " vao " << bp.getVao() << " with Chunk::cubeBlueprint vao " << Chunk::cubeBlueprint->getVao() << std::endl)
	GLuint	vao = bp->getVao();
	if (vao == 0) {
		D("vao: " << vao << "\n")
		Misc::breakExit(66);
	}
	glBindVertexArray(vao);

	if (obj->displayTexture && obj->getTexture() != nullptr) {
		glUniform1f(this->_tex_coef, 1.0f);
		glActiveTexture(GL_TEXTURE0);//required for some drivers
		glBindTexture(GL_TEXTURE_2D, obj->getTexture()->getId());
	} else { glUniform1f(this->_tex_coef, 0.0f); }

	glPolygonMode(GL_FRONT_AND_BACK, obj->getPolygonMode());
	int	vertices_amount = bp->getPolygonAmount() * 3;
	//D(vertices_amount << "_")

	/**/
	if (bp->getDataMode() == BP_LINEAR)
		glDrawArrays(GL_TRIANGLES, 0, vertices_amount);
	else // should be BP_INDICES
		glDrawElements(GL_TRIANGLES, vertices_amount, GL_UNSIGNED_INT, 0);
	/**/

	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void	Obj3dPG::renderUniqueId(Obj3d& obj, Math::Matrix4 PVmatrix) const {
	Obj3dBP* bp = obj.getBlueprint();
	Math::Matrix4& modelMatrix = obj.getWorldMatrix();

	//D("rendering " << bp.getName() << " #" << obj.getId() << " vao:" << bp.getVao() << std::endl)
	//D("*\tpolygons: " << bp.getPolygonAmount() << std::endl)

	PVmatrix.mult(modelMatrix);
	PVmatrix.setOrder(COLUMN_MAJOR);

	glUniformMatrix4fv(this->_mat4_mvp, 1, GL_FALSE, PVmatrix.getData());
	glUniform1i(this->_dismod, 1);// 1 = display plain_color, 0 = vertex_color
	//plain_color should not be used, check shader
	unsigned int id = obj.getId();
	uint8_t	rgb[3];
	Misc::intToRGB(id, &(*rgb));
	glUniform3f(this->_plain_color, float(rgb[0]) / 255.0f, float(rgb[1]) / 255.0f, float(rgb[2]) / 255.0f);
	//should store address of object directly, if we can store RGBA with the shader

	glBindVertexArray(bp->getVao());
	glUniform1f(this->_tex_coef, 0.0f);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glDrawArrays(GL_TRIANGLES, 0, bp->getPolygonAmount() * 3);

	glBindVertexArray(0);
}

void	Obj3dPG::renderObjects(std::list<Object*>& list, Cam& cam, unsigned int flags) {
	//D("render all Obj3d" << std::endl)
	if (list.empty())
		return;
	//assuming all Obj3d have the same program
	glUseProgram(this->_program);//used once for all obj3d
	Math::Matrix4	viewProMatrix(cam.getProjectionMatrix());
	Math::Matrix4	viewMatrix = cam.getViewMatrix();
	viewProMatrix.mult(viewMatrix);

	bool			draw = true;
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
			D("dynamic_cast<Obj3d*> failed on Object : " << o << std::endl)
			Misc::breakExit(22);
			return;
		}
		else {
			object->update();
			draw = true;
			oldcolor = object->getColor();
			color = oldcolor;
			if (flags & PG_FRUSTUM_CULLING) {
				if (object->getParent()) {
					worldmatrix = object->getWorldMatrix();
					worldmatrix.setOrder(COLUMN_MAJOR);
					float* m = worldmatrix.getData();
					center = Math::Vector3(m[12], m[13], m[14]);//world pos and not real center
				}
				else {
					center = object->local.getPos();//== world pos
				}
				scale = object->local.getScale();//what if parent?
				dim = object->getBlueprint()->getDimensions();//what if parent?
				dim.x *= scale.x;
				dim.y *= scale.y;
				dim.z *= scale.z;
				center.add(dim.x / 2.0f, dim.y / 2.0f, dim.z / 2.0f);//what about the rotation
				/*
					get the center of the object BP, apply the rotation, add up to the world pos
					for the voxel engine, it doesnt matter since the voxels dont have parent or rotation
				*/
				if (cam.isInFrustum(center, viewProMatrix)) {
					//color = Math::Vector3(40, 200, 200);//cyan
					counterFrustum++;
				}
				else if (cam.local.forwardDistance(center) > 0) {
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
			}

			if (flags & PG_FORCE_DRAW || draw) {
				object->setColor(color.x, color.y, color.z);
				object->render(viewProMatrix);//use the object renderer, might be different depending on the object
				//this->render(*object, viewProMatrix);//use this renderer for all objects
				object->setColor(oldcolor.x, oldcolor.y, oldcolor.z);
			}
		}
	}
	//D("fustrum objects: " << counterFrustum << std::endl)
	//D("forward objects: " << counterForward << "\t(not in fustrum)" << std::endl)
	//D("total objects: " << list.size() << std::endl)
	//D(std::endl)
	for (Object* object : list) {//to do AFTER all objects are rendered
		//D("rendered object: " << object->getId() << ", bp vao: " << ((Obj3d*)object)->getBlueprint().getVao() << "\n")
		object->local._matrixChanged = false;
		object->_worldMatrixChanged = false;
	}
}

void	Obj3dPG::renderObjects(Object** objectArray, Cam& cam, unsigned int flags) {
	//D("render all Obj3d" << std::endl)
	if (!objectArray || !objectArray[0])
		return;
	//assuming all Obj3d have the same program
	glUseProgram(this->_program);//used once for all obj3d
	Obj3d* object = nullptr;
	Math::Matrix4	viewProMatrix(cam.getProjectionMatrix());
	Math::Matrix4	viewMatrix = cam.getViewMatrix();
	viewProMatrix.mult(viewMatrix);

	bool			draw = true;
	unsigned int	counterForward = 0;
	unsigned int	counterFrustum = 0;
	Math::Vector3	center;
	Math::Vector3	dim;
	Math::Vector3	scale;
	Math::Vector3	oldcolor;
	Math::Vector3	color;
	Math::Matrix4	worldmatrix;

	unsigned int	i = 0;
	while (objectArray[i]) {
		object = dynamic_cast<Obj3d*>(objectArray[i]);
		if (!object) {
			D("dynamic_cast<Obj3d*> failed on Object : " << objectArray[i] << std::endl)
			Misc::breakExit(22);
			return;
		}
		else {
			object->update();
			draw = true;
			oldcolor = object->getColor();
			color = oldcolor;
			if (flags & PG_FRUSTUM_CULLING) {
				if (object->getParent()) {
					worldmatrix = object->getWorldMatrix();
					worldmatrix.setOrder(COLUMN_MAJOR);
					float* m = worldmatrix.getData();
					center = Math::Vector3(m[12], m[13], m[14]);//world pos and not real center
				}
				else {
					center = object->local.getPos();//== world pos
				}
				scale = object->local.getScale();//what if parent?
				dim = object->getBlueprint()->getDimensions();//what if parent?
				dim.x *= scale.x;
				dim.y *= scale.y;
				dim.z *= scale.z;
				center.add(dim.x / 2.0f, dim.y / 2.0f, dim.z / 2.0f);//what about the rotation
				/*
					get the center of the object BP, apply the rotation, add up to the world pos
					for the voxel engine, it doesnt matter since the voxels dont have parent or rotation
				*/
				if (cam.isInFrustum(center, viewProMatrix)) {
					//color = Math::Vector3(40, 200, 200);//cyan
					counterFrustum++;
				}
				else if (cam.local.forwardDistance(center) > 0) {
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
			}

			if (flags & PG_FORCE_DRAW || draw) {
				object->setColor(color.x, color.y, color.z);
				object->render(viewProMatrix);//use the object renderer, might be different depending on the object
				//this->render(*object, viewProMatrix);//use this renderer for all objects
				object->setColor(oldcolor.x, oldcolor.y, oldcolor.z);
			}
		}
		i++;
	}

	//D("fustrum objects: " << counterFrustum << std::endl)
	//D("forward objects: " << counterForward << "\t(not in fustrum)" << std::endl)
	//D("total objects: " << list.size() << std::endl)
	//D(std::endl)
	i = 0;
	while (objectArray[i]) {//to do AFTER all objects are rendered
		//D("rendered object: " << object->getId() << ", bp vao: " << ((Obj3d*)object)->getBlueprint().getVao() << "\n")
		objectArray[i]->local._matrixChanged = false;
		objectArray[i]->_worldMatrixChanged = false;
		i++;
	}
}

void	Obj3dPG::renderObjectsMultiDraw(std::list<Object*>& list, Cam& cam, unsigned int flags) {
	//D("render all Obj3d" << std::endl)
	if (list.empty())
		return;
	//assuming all Obj3d have the same program
	glUseProgram(this->_program);//used once for all obj3d
	Math::Matrix4	viewProMatrix(cam.getProjectionMatrix());
	Math::Matrix4	viewMatrix = cam.getViewMatrix();
	viewProMatrix.mult(viewMatrix);

	Obj3d* front_obj = dynamic_cast<Obj3d*>(list.front());
	Obj3dBP* front_bp = front_obj->getBlueprint();
	viewProMatrix.mult(front_obj->getWorldMatrix());
	viewProMatrix.setOrder(COLUMN_MAJOR);

	if (front_bp->getDataMode() == BP_LINEAR) {
		//D("PG_MULTIDRAW BP_LINEAR\n")
		unsigned int drawcount = list.size();
		GLsizei* vertices_amount_array = new GLsizei[drawcount];// GLsizei = int
		GLint* start_offsets = new GLint[drawcount];
		unsigned int x = 0;
		for (Object* o : list) {
			Obj3d* object = dynamic_cast<Obj3d*>(o);
			if (!object) {
				D("dynamic_cast<Obj3d*> failed on Object : " << o << std::endl)
				Misc::breakExit(22);
				return;
			}
			object->update();

			const Obj3dBP* bp = object->getBlueprint();

			vertices_amount_array[x] = bp->getPolygonAmount() * 3;
			start_offsets[x] = 0;
			//D((int)vertices_amount_array[x] << " ")
			x++;
		}


		//modelMatrix->printData();

		//D("rendering " << bp.getName() << " #" << obj->getId() << " vao:" << bp.getVao() << std::endl)
		//D("*\tpolygons: " << bp.getPolygonAmount() << std::endl)


		glUniformMatrix4fv(this->_mat4_mvp, 1, GL_FALSE, viewProMatrix.getData());
		glUniform1i(this->_dismod, 0);// 1 = display plain_color, 0 = vertex_color (.mtl)
		//glUniform3f(this->_plain_color, color.x, color.y, color.z);

		//glBindVertexArray(bp->getVao());
		glUniform1f(this->_tex_coef, 0.0f);
		glPolygonMode(GL_FRONT_AND_BACK, front_obj->getPolygonMode());
		D("3")

		glMultiDrawArrays(GL_TRIANGLES, start_offsets, vertices_amount_array, drawcount);
		//glMultiDrawArrays(GL_TRIANGLES, vertices_amount_array, GL_UNSIGNED_INT, (void**)indices_2d_array, drawcount);
		D("4")

		glBindVertexArray(0);
		glBindTexture(GL_TEXTURE_2D, 0);

	}
	else if (front_bp->getDataMode() == BP_INDICES) {
		//D("PG_MULTIDRAW BP_INDICES\n")
		unsigned int drawcount = list.size();
		GLsizei* vertices_amount_array = new GLsizei[drawcount];// GLsizei = int
		unsigned int** indices_2d_array = new unsigned int* [drawcount];
		unsigned int x = 0;
		for (Object* o : list) {
			Obj3d* object = dynamic_cast<Obj3d*>(o);
			if (!object) {
				D("dynamic_cast<Obj3d*> failed on Object : " << o << std::endl)
				Misc::breakExit(22);
				return;
			}
			object->update();

			const Obj3dBP* bp = object->getBlueprint();

			vertices_amount_array[x] = bp->getPolygonAmount() * 3;
			indices_2d_array[x] = bp->getIndices().data();
			//D((int)vertices_amount_array[x] << " ")
			x++;
		}
		D(">")
		//glMultiDrawElementsIndirect(GL_TRIANGLES, );

		if (!front_obj) {
			D("dynamic_cast<Obj3d*> failed on Object : " << front_obj << std::endl)
			Misc::breakExit(22);
			return;
		}
		Math::Matrix4* modelMatrix = &front_obj->getWorldMatrix();
		//modelMatrix->printData();

		//D("rendering " << bp.getName() << " #" << obj->getId() << " vao:" << bp.getVao() << std::endl)
		//D("*\tpolygons: " << bp.getPolygonAmount() << std::endl)

		viewProMatrix.mult(*modelMatrix);
		viewProMatrix.setOrder(COLUMN_MAJOR);

		//can be done once for all obj3d
		glUseProgram(this->_program);

		glUniformMatrix4fv(this->_mat4_mvp, 1, GL_FALSE, viewProMatrix.getData());
		glUniform1i(this->_dismod, 0);// 1 = display plain_color, 0 = vertex_color (.mtl)
		//glUniform3f(this->_plain_color, color.x, color.y, color.z);

		//glBindVertexArray(front_obj->getBlueprint()->getVao());
		glUniform1f(this->_tex_coef, 0.0f);
		glPolygonMode(GL_FRONT_AND_BACK, front_obj->getPolygonMode());
		D("3")

		glMultiDrawElements(GL_TRIANGLES, vertices_amount_array, GL_UNSIGNED_INT, (void**)indices_2d_array, drawcount);
		D("4")

		glBindVertexArray(0);
		glBindTexture(GL_TEXTURE_2D, 0);

		delete vertices_amount_array;
		delete indices_2d_array;
	}
	else {
		D("Error: Unknow data mode.\n")
		Misc::breakExit(56);
	}
	
	//D("fustrum objects: " << counterFrustum << std::endl)
	//D("forward objects: " << counterForward << "\t(not in fustrum)" << std::endl)
	//D("total objects: " << list.size() << std::endl)
	//D(std::endl)
	for (Object* object : list) {//to do AFTER all objects are rendered
		//D("rendered object: " << object->getId() << ", bp vao: " << ((Obj3d*)object)->getBlueprint().getVao() << "\n")
		object->local._matrixChanged = false;
		object->_worldMatrixChanged = false;
	}
}
void	Obj3dPG::renderObjectsMultiDraw(Object** objectArray, Cam& cam, unsigned int flags) {
	D("Error : empty function :\n" << __PRETTY_FUNCTION__)
	Misc::breakExit(5664);
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

	D("_ " << __PRETTY_FUNCTION__ << " : " << this->_program << std::endl)
	this->_mat4_mvp = this->getSlot("MVP", true);
	this->_dismod = this->getSlot("dismod", true);
	this->_plain_color = this->getSlot("plain_color", true);
	this->_tex_coef = this->getSlot("tex_coef", true);

	this->_vertex_position_data = this->getSlot("vertex_position_data", false);
	this->_vertex_UV_data = this->getSlot("vertex_UV_data", false);
	this->_vertex_color_data = this->getSlot("vertex_color_data", false);
}
