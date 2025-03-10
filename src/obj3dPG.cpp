#include "simplegl.h"
#include "obj3dPG.hpp"
#include "compiler_settings.h"

//#define OBJ3D_RENDER_CHECKS
#ifndef OBJ3D_RENDER_CHECKS
#pragma message("[Obj3dPG.hpp] OBJ3D_RENDER_CHECKS is not defined. C style casts, no vao checks.")
/*
[[deprecated("[Obj3dPG.hpp] OBJ3D_RENDER_CHECKS is defined. C style casts, no vao checks.")]]
inline int	custom_warning() { return -1; }
static int ret = custom_warning();
*/
#endif

#define SGL_DEBUG
#ifdef SGL_DEBUG
 #define SGL_OBJ3DPG_DEBUG
#endif
#ifdef SGL_OBJ3DPG_DEBUG 
 #define D(x) std::cout << "[Obj3dPG] " << x
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
	this->_dismod = 0;
	this->_mat4_mvp = 0;
	this->_plain_color = 0;
	this->_tex_coef = 0;
	this->_vertex_position_data = 0;
	this->_vertex_color_data = 0;
	this->_vertex_UV_data = 0;

	//D(__PRETTY_FUNCTION__ << std::endl);
	if (init_locations)
		this->getLocations();
	//D(__PRETTY_FUNCTION__ << " END" << std::endl);
}

Obj3dPG::~Obj3dPG() {
	//D(__PRETTY_FUNCTION__ << std::endl);
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

// Directly render a model without checking anything (frustum, front, flags, etc)
void	Obj3dPG::renderObject(Object& object, Math::Matrix4 PVmatrix) const {
	#ifdef OBJ3D_RENDER_CHECKS
	Obj3d* obj = dynamic_cast<Obj3d*>(&object);
	if (!obj) {
		D("dynamic_cast<Obj3d*> failed on Object : " << obj << std::endl);
		// Misc::breakExit(22);
		// this can happen when an object is being nulled to be replaced by another one, block or continue?
		return;
	}
	Obj3dBP* bp = obj->getBlueprint();
	Obj3dBP* bp0 = dynamic_cast<Obj3dBP*>(bp->lodManager.getCurrentLodBlueprint());
	if (!bp0) {
		//meaning a LOD has a different BP type, could be for different shaders...
		D("dynamic_cast<Obj3d*> failed on Blueprint : " << bp << std::endl);
		return;
	}
	bp = bp0;
	GLuint					vao = bp->getVao();
	if (vao == 0) {
		D("bp " << bp << " vao: " << vao << "\n");
		Misc::breakExit(66);
	}
	#else
	Obj3d*		obj = (Obj3d*)&object;
	Obj3dBP*	bp = (Obj3dBP*)obj->getBlueprint()->lodManager.getCurrentLodBlueprint();
	GLuint		vao = bp->getVao();
	#endif
	Math::Matrix4&			modelMatrix = obj->getWorldMatrix();
	unsigned int			vertices_amount = bp->getPolygonAmount() * 3;
	const Math::Vector3		color = obj->getColorShader();

	PVmatrix.mult(modelMatrix);
	PVmatrix.setOrder(COLUMN_MAJOR);
	//D("rendering " << bp.getName() << " #" << obj->getId() << " vao:" << bp.getVao() << std::endl);
	//D("*\tpolygons: " << bp.getPolygonAmount() << std::endl);


	//can be done once for all obj3d
	glUseProgram(this->_program);
	#ifdef PG_FORCE_LINKBUFFERS
	this->linkBuffers(*bp);//needed for LOD BPs. This should be done only once, it's currently done in the Obj3d creation. It's in fact unrelated... Should be in the BP
	#endif

	glUniformMatrix4fv(this->_mat4_mvp, 1, GL_FALSE, PVmatrix.getData());
	glUniform1i(this->_dismod, 0);// 1 = display plain_color, 0 = vertex_color (.mtl)
	glUniform3f(this->_plain_color, color.x, color.y, color.z);

	glBindVertexArray(vao);
	if (obj->displayTexture && obj->getTexture() != nullptr) {
		glUniform1f(this->_tex_coef, 1.0f);
		glActiveTexture(GL_TEXTURE0);//required for some drivers
		glBindTexture(GL_TEXTURE_2D, obj->getTexture()->getId());
	} else { glUniform1f(this->_tex_coef, 0.0f); }
	glPolygonMode(GL_FRONT_AND_BACK, obj->getPolygonMode()); //GL_FRONT & GL_BACK are deprecated for this method

	/**/
	if (bp->getDataMode() == BP_LINEAR)
		glDrawArrays(GL_TRIANGLES, 0, vertices_amount);
	else // should be BP_INDICES
		glDrawElements(GL_TRIANGLES, vertices_amount, GL_UNSIGNED_INT, 0);
	/**/

	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void	Obj3dPG::renderObjectUniqueId(Obj3d& obj, Math::Matrix4 PVmatrix) const {
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

static void		frustumCulling_stats(bool* draw, Obj3d* object, Cam* cam, unsigned int flags, Math::Matrix4& viewProMatrix,
	unsigned int* counterForward, unsigned int* counterFrustum, Math::Vector3* color)//tmp debug params
{
	Math::Matrix4	worldmatrix;
	Math::Vector3	center;
	Math::Vector3	scale;
	Math::Vector3	dim;
	Math::Vector3	oldcolor;

	oldcolor = object->getColor();
	if (object->getParent()) {
		worldmatrix = object->getWorldMatrix();
		worldmatrix.setOrder(COLUMN_MAJOR);
		float* m = worldmatrix.getData();
		center = Math::Vector3(m[12], m[13], m[14]);//world pos and not real center
		//todo : this is probably false due to rotation and other things, check that
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
	if (cam->isInFrustum(center, viewProMatrix)) {
		//color = Math::Vector3(40, 200, 200);//cyan
		(*counterFrustum)++;
	}
	else if (cam->local.forwardDistance(center) > 0) {
		*color = Math::Vector3(200, 200, 40);//yellow
		(*counterForward)++;
		*draw = false;
	}
	else {
		*draw = false;
	}
	if (object->getPolygonMode() == GL_LINE) {
		*color = oldcolor;
	}
}

// returns true if objects is inside frustum
static bool		frustumCulling( Obj3d* object, Cam* cam, unsigned int flags, Math::Matrix4& viewProMatrix) {
	Math::Matrix4	worldmatrix;
	Math::Vector3	center;
	Math::Vector3	scale;
	Math::Vector3	dim;
	bool			isInside = true;

	if (object->getParent()) {
		worldmatrix = object->getWorldMatrix();
		worldmatrix.setOrder(COLUMN_MAJOR);
		float* m = worldmatrix.getData();
		center = Math::Vector3(m[12], m[13], m[14]);//world pos and not real center
		//todo : this is probably false due to rotation and other things, check that
	} else {
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
	if (!cam->isInFrustum(center, viewProMatrix)) {
		isInside = false;
		if (cam->local.forwardDistance(center) > 0) {//in not behind (180� angle)
			// ...
		}
	}
	return isInside;
}

void		Obj3dPG::renderAllObjects(std::vector<Object*>& objects, Cam& cam, unsigned int flags) {
	//D("render all Obj3d" << std::endl);
	if (objects.empty())
		return;
	glUseProgram(this->_program);//used once for all obj3d, assuming they have the same program

	Math::Matrix4	viewProMatrix = cam.getProjectionMatrix();
	Math::Matrix4	viewMatrix = cam.getViewMatrix();
	viewProMatrix.mult(viewMatrix);
	bool			draw = true;
	t_pp			cam_pp = Math::extractFromMatrix(cam.getWorldMatrix());

	for (Object* obj : objects) {
		obj->update();
	}
	for (Object* obj : objects) {
		obj->local._matrixChanged = false;//to do AFTER all objects are updated
		obj->_worldMatrixChanged = false;//to do AFTER all objects are updated

		Obj3d* model = dynamic_cast<Obj3d*>(obj);
		if (!model) {
			D("dynamic_cast<Obj3d*> failed on Object : " << obj << std::endl);
			Misc::breakExit(22);
			return;
		}
		else {
			if (flags & PG_FRUSTUM_CULLING)
				draw = frustumCulling(model, &cam, flags, viewProMatrix);
			if (draw || flags & PG_FORCE_DRAW) {
				float	distanceFromCam = (cam_pp.pos - model->local.getPos()).len();
				model->getBlueprint()->lodManager.updateCurrentLod(distanceFromCam);
				this->renderObject(*model, viewProMatrix);//use this renderer for all models
			}
		}
	}
}

void	Obj3dPG::renderAllObjects_stats(std::vector<Object*>& objects, Cam& cam, unsigned int flags) {
	//D("render all Obj3d" << std::endl);
	if (objects.empty())
		return;
	//assuming all Obj3d have the same program
	glUseProgram(this->_program);//used once for all obj3d
	Math::Matrix4	viewProMatrix = cam.getProjectionMatrix();
	Math::Matrix4	viewMatrix = cam.getViewMatrix();
	viewProMatrix.mult(viewMatrix);

	bool			draw = true;
	unsigned int	counterForward = 0;
	unsigned int	counterFrustum = 0;
	Math::Vector3	oldcolor;
	Math::Vector3	color;
	t_pp			cam_pp = Math::extractFromMatrix(cam.getWorldMatrix());

	for (Object* o : objects) {
		Obj3d* object = dynamic_cast<Obj3d*>(o);
		if (!object) {
			D("dynamic_cast<Obj3d*> failed on Object : " << o << std::endl);
			Misc::breakExit(22);
			return;
		}
		else {
			object->update();
			draw = true;
			oldcolor = object->getColor();
			color = oldcolor;

			if (flags & PG_FRUSTUM_CULLING)
				frustumCulling_stats(&draw, object, &cam, flags, viewProMatrix, &counterForward, &counterFrustum, &color);

			if (flags & PG_FORCE_DRAW || draw) {
				float	distanceFromCam = (cam_pp.pos - object->local.getPos()).len();
				object->getBlueprint()->lodManager.updateCurrentLod(distanceFromCam);

				object->setColor(color.x, color.y, color.z);
				object->render(viewProMatrix);//use the object renderer, might be different depending on the object
				//this->render(*object, viewProMatrix);//use this renderer for all objects

				//restore old values
				object->setColor(oldcolor.x, oldcolor.y, oldcolor.z);
			}
		}
	}
	if (flags & PG_FRUSTUM_CULLING) {
		//D("frustum objects: " << counterFrustum << std::endl);
		//D("forward objects: " << counterForward << "\t(not in frustum)" << std::endl);
		//D("total objects: " << objects.size() << std::endl);
		//D(std::endl);
	}
	for (Object* object : objects) {//to do AFTER all objects are rendered
		//D("rendered object: " << object->getId() << ", bp vao: " << ((Obj3d*)object)->getBlueprint().getVao() << "\n");
		object->local._matrixChanged = false;
		object->_worldMatrixChanged = false;
	}
}

void	Obj3dPG::renderAllObjectsMultiDraw(std::vector<Object*>& objects, Cam& cam, unsigned int flags) {
	//D("render all Obj3d" << std::endl)
	if (objects.empty())
		return;
	//assuming all Obj3d have the same program
	glUseProgram(this->_program);//used once for all obj3d
	Math::Matrix4	viewProMatrix(cam.getProjectionMatrix());
	Math::Matrix4	viewMatrix = cam.getViewMatrix();
	viewProMatrix.mult(viewMatrix);

	Obj3d* front_obj = dynamic_cast<Obj3d*>(objects.front());
	Obj3dBP* front_bp = front_obj->getBlueprint();
	viewProMatrix.mult(front_obj->getWorldMatrix());
	viewProMatrix.setOrder(COLUMN_MAJOR);

	if (front_bp->getDataMode() == BP_LINEAR) {
		//D("PG_MULTIDRAW BP_LINEAR\n")
		size_t drawcount = objects.size();
		GLsizei* vertices_amount_array = new GLsizei[drawcount];// GLsizei = int
		GLint* start_offsets = new GLint[drawcount];
		unsigned int x = 0;
		for (Object* o : objects) {
			Obj3d* object = dynamic_cast<Obj3d*>(o);
			if (!object) {
				D("dynamic_cast<Obj3d*> failed on Object : " << o << std::endl);
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
		D("3");

		glMultiDrawArrays(GL_TRIANGLES, start_offsets, vertices_amount_array, drawcount);
		//glMultiDrawArrays(GL_TRIANGLES, vertices_amount_array, GL_UNSIGNED_INT, (void**)indices_2d_array, drawcount);
		D("4");

		glBindVertexArray(0);
		glBindTexture(GL_TEXTURE_2D, 0);

		delete[] vertices_amount_array;
		delete[] start_offsets;
	}
	else if (front_bp->getDataMode() == BP_INDICES) {
		//D("PG_MULTIDRAW BP_INDICES\n")
		size_t drawcount = objects.size();
		GLsizei* vertices_amount_array = new GLsizei[drawcount];// GLsizei = int
		unsigned int** indices_2d_array = new unsigned int* [drawcount];
		unsigned int x = 0;
		for (Object* o : objects) {
			Obj3d* object = dynamic_cast<Obj3d*>(o);
			if (!object) {
				D("dynamic_cast<Obj3d*> failed on Object : " << o << std::endl);
				Misc::breakExit(22);
				return;
			}
			object->update();

			Obj3dBP* bp = object->getBlueprint();

			vertices_amount_array[x] = bp->getPolygonAmount() * 3;
			indices_2d_array[x] = bp->getIndices().data();
			//D((int)vertices_amount_array[x] << " ")
			x++;
		}
		D(">");
		//glMultiDrawElementsIndirect(GL_TRIANGLES, );

		if (!front_obj) {
			D("dynamic_cast<Obj3d*> failed on Object : " << front_obj << std::endl);
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
		D("3");

		glMultiDrawElements(GL_TRIANGLES, vertices_amount_array, GL_UNSIGNED_INT, (void**)indices_2d_array, drawcount);
		D("4");

		glBindVertexArray(0);
		glBindTexture(GL_TEXTURE_2D, 0);

		delete[] vertices_amount_array;
		delete[] indices_2d_array;
	}
	else {
		D("Error: Unknow data mode.\n");
		Misc::breakExit(56);
	}
	
	//D("frustum objects: " << counterFrustum << std::endl)
	//D("forward objects: " << counterForward << "\t(not in frustum)" << std::endl)
	//D("total objects: " << list.size() << std::endl)
	//D(std::endl)
	for (Object* object : objects) {//to do AFTER all objects are rendered
		//D("rendered object: " << object->getId() << ", bp vao: " << ((Obj3d*)object)->getBlueprint().getVao() << "\n")
		object->local._matrixChanged = false;
		object->_worldMatrixChanged = false;
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

	//D("_ " << __PRETTY_FUNCTION__ << " : " << this->_program << std::endl);
	this->_mat4_mvp = this->getSlot("MVP", true);
	this->_dismod = this->getSlot("dismod", true);
	this->_plain_color = this->getSlot("plain_color", true);
	this->_tex_coef = this->getSlot("tex_coef", true);

	this->_vertex_position_data = this->getSlot("vertex_position_data", false);
	this->_vertex_UV_data = this->getSlot("vertex_UV_data", false);
	this->_vertex_color_data = this->getSlot("vertex_color_data", false);
}
