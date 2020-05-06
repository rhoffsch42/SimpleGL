#include "renderer.hpp"

void	Renderer::blitToWindow(FrameBuffer* readFramebuffer, GLenum attachmentPoint, UIPanel* panel) {
	GLuint fbo;
	if (readFramebuffer) {
		fbo = readFramebuffer->fbo;
	}
	else {
		fbo = panel->getFbo();
	}
	// glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
	glBindFramebuffer(GL_READ_FRAMEBUFFER, fbo);

	//glViewport(0, 0, manager->glfw->getWidth(), manager->glfw->getHeight());//size of the window/image or panel width ?
	glReadBuffer(attachmentPoint);
	GLenum drawBuffers[1] = { GL_COLOR_ATTACHMENT0 };
	glDrawBuffers(1, drawBuffers);

	int w;
	int h;
	if (readFramebuffer) {
		w = readFramebuffer->getWidth();
		h = readFramebuffer->getHeight();
	}
	else if (panel->getTexture()) {
		w = panel->getTexture()->getWidth();
		h = panel->getTexture()->getHeight();
	}
	else {
		std::cout << "FUCK " << __PRETTY_FUNCTION__ << std::endl;
		exit(2);
	}
	if (0) {
		std::cout << "copy " << w << "x" << h << "\tresized\t" << panel->_width << "x" << panel->_height \
			<< "\tat pos\t" << panel->_posX << ":" << panel->_posY << std::endl;
		// << " -> " << (panel->posX + panel->width) << "x" << (panel->posY + panel->height) << std::endl;
	}
	glBlitFramebuffer(0, 0, w, h, \
		panel->_posX, panel->_posY, panel->_posX2, panel->_posY2, GL_COLOR_BUFFER_BIT, GL_NEAREST);

	glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);
}
////////////////////////////////////////////////////////////////////

float	Renderer::isForward(Properties& local_referenciel, Math::Vector3 target_pos) {//upgrade this to isInFrustum
	//return 1;
	Math::Vector3	ref_pos(local_referenciel.getPos());
	Math::Rotation	ref_rot(local_referenciel.getRot());
	Math::Vector3	diff(target_pos);
	diff.sub(ref_pos);

	Math::Vector3	right = VEC3_RIGHT;
	Math::Vector3	up = VEC3_UP;
	right.ZYXrotate(ref_rot, -ROT_WAY);
	up.ZYXrotate(ref_rot, -ROT_WAY);
	Math::Vector3	forward = Math::Vector3::cross(up, right);

	return Math::Vector3::dot(forward, diff);
}

void	Renderer::iqFrustumF_CreatePerspective(float* frus, float fovy, float aspect, float znear, float zfar) {
	//from http ://www.iquilezles.org/www/articles/frustum/frustum.htm
	const float an = fovy / 2.0f * (3.141592653589f / 180.0f);//corrected
	const float si = sinf(an);
	const float co = cosf(an);
	frus[0] = 0.0f;		frus[1] = -co;		frus[2] = si;			frus[3] = 0.0f;//top
	frus[4] = 0.0f;		frus[5] = co;		frus[6] = si;			frus[7] = 0.0f;//bottom
	frus[8] = co;		frus[9] = 0.0f;		frus[10] = si * aspect;	frus[11] = 0.0f;//left
	frus[12] = -co;		frus[13] = 0.0f;	frus[14] = si * aspect;	frus[15] = 0.0f;//right
	frus[16] = 0.0f;	frus[17] = 0.0f;	frus[18] = 1.0f;		frus[19] = zfar;//far
	frus[20] = 0.0f;	frus[21] = 0.0f;	frus[22] = -1.0f;		frus[23] = -znear;//near

	//corrected signs...
	frus[0] = 0.0f;		frus[1] = -co;		frus[2] = -si;			frus[3] = 0.0f;//top
	frus[4] = 0.0f;		frus[5] = co;		frus[6] = -si;			frus[7] = 0.0f;//bottom
	frus[8] = co;		frus[9] = 0.0f;		frus[10] = -si * aspect;	frus[11] = 0.0f;//left
	frus[12] = -co;		frus[13] = 0.0f;	frus[14] = -si * aspect;	frus[15] = 0.0f;//right
	frus[16] = 0.0f;	frus[17] = 0.0f;	frus[18] = 1.0f;		frus[19] = zfar;//far
	frus[20] = 0.0f;	frus[21] = 0.0f;	frus[22] = -1.0f;		frus[23] = -znear;//near
}

//target_pos must be in cam space
bool	Renderer::isInFrustum(float* frustum, Math::Vector3 target_pos) {
	int	in = 0;
	size_t	max = 4;// 6 = all planes, 4 = no far no near
	for (size_t i = 0; i < max; i++) {
		Math::Vector3	normal(frustum[i * 4 + 0], frustum[i * 4 + 1], frustum[i * 4 + 2]);
		if (Math::Vector3::dot(normal, target_pos) > 0)
			in++;
	}
	//manual for near and far
	max += 2;
	if (target_pos.z > -frustum[19])
		in++;
	if (target_pos.z < frustum[23])
		in++;
	return ((in == max) ? true : false);//is on the right halfspace of all planes, ie. is in the frustum
}

////////////////////////////////////////////////////////

Renderer::Renderer(std::string mesh_vs, std::string mesh_fs, std::string cubemap_vs, std::string cubemap_fs) {
	this->meshProgram = new Obj3dPG(mesh_vs, mesh_fs);
	this->skyboxProgram = new SkyboxPG(cubemap_vs, cubemap_fs);
}

Renderer::~Renderer() {
	delete this->meshProgram;
	delete this->skyboxProgram;
}

void	Renderer::renderObj3d(list<Obj3d*>& obj3dList, Cam& cam, bool force_draw) {//make a renderObj3d for only 1 obj
	// cout << "render all Obj3d" << endl;
	//assuming all Obj3d have the same program
	if (obj3dList.empty())
		return;
	glUseProgram(this->meshProgram->_program);//used once for all obj3d
	Math::Matrix4	proMatrix(cam.getProjectionMatrix());
	Math::Matrix4	viewMatrix = cam.getViewMatrix();
	proMatrix.mult(viewMatrix);// do it in shader ? NO cauz shader will do it for every vertix

	float			frustum[24];
	iqFrustumF_CreatePerspective(frustum, cam.getFov(), float(WINX) / float(WINY), cam.getNear(), cam.getFar());//todo save aspect ratio in Cam::
	unsigned int	counterForward = 0;
	unsigned int	counterFrustum = 0;
	Math::Vector3	center;
	Math::Vector3	dim;
	Math::Vector3	centerCamSpace;
	Math::Vector3	oldcolor;
	Math::Vector3	color;

	for (Obj3d* object : obj3dList) {
		center = object->local.getPos();
		dim = object->local.getScale();
		center.add(dim.x / 2.0f, dim.y / 2.0f, dim.z / 2.0f);
		centerCamSpace = center;
		centerCamSpace.mult(proMatrix);// transform object pos to camera space

		bool draw = true;
		oldcolor = object->getColor();
		color = oldcolor;
		if (isInFrustum(frustum, centerCamSpace)) {
			//color = Math::Vector3(40, 200, 200);//cyan
			counterFrustum++;
		} else if (isForward(cam.local, center) > 0) {
			color = Math::Vector3(200, 200, 40);//yellow
			counterForward++;
			draw = false;
		} else {
			draw = false;
		}

		if (object->getPolygonMode() == GL_LINE) {
			color = oldcolor;
		}

		if (force_draw || draw) {
			object->setColor(color.x, color.y, color.z);
			object->render(proMatrix);
			object->setColor(oldcolor.x, oldcolor.y, oldcolor.z);
		}

	}
	if (0) {
		std::string		planes[6] = { "top", "bottom", "left", "right", "far", "near" };
		for (size_t i = 0; i < 6; i++) {
			Math::Vector3	normal(frustum[i * 4 + 0], frustum[i * 4 + 1], frustum[i * 4 + 2]);
			std::cout << "normal " << planes[i] << "\t"; normal.printData();
		}
	}
	//std::cout << "fustrum objects: " << counterFrustum << std::endl;
	//std::cout << "forward objects: " << counterForward << "\t(not in fustrum)" << std::endl;
	//std::cout << "total objects: " << obj3dList.size() << std::endl;
	//std::cout << std::endl;
	for (Obj3d* object : obj3dList) {
		object->local._matrixChanged = false;
		object->_worldMatrixChanged = false;
	}
}

void	Renderer::renderSkybox(Skybox& skybox, Cam& cam) {
	// cout << "render Skybox" << endl;
	glUseProgram(this->skyboxProgram->_program);

	Math::Matrix4	proMatrix(cam.getProjectionMatrix());
	Math::Matrix4& viewMatrix = cam.getViewMatrix();
	proMatrix.mult(viewMatrix);

	skybox.render(proMatrix);
}
