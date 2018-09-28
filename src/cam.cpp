#include "simplegl.h"
#include "cam.hpp"

Cam::Cam(Glfw& glfw) : Object() {
	cout << "_ Cam Cons default" << endl;
	this->_fov = CAM_FOV;
	this->_near = CAM_NEAR;
	this->_far = CAM_FAR;
	this->updateCamVectors();
	this->_projectionMatrix.projectionMatrix(Math::toRadian(this->_fov), this->_far, this->_near, glfw._width, glfw._height);
	this->_viewMatrix.viewMatrix(this->local._pos, this->local._rot);
}
Cam::Cam(Glfw& glfw, Math::Vector3 pos, Math::Rotation rot) : Object() {
	cout << "_ Cam Cons (pos, rot)" << endl;
	this->_fov = CAM_FOV;
	this->_near = CAM_NEAR;
	this->_far = CAM_FAR;
	this->local._pos = pos;
	this->local._rot = rot;
	this->updateCamVectors();
	this->_projectionMatrix.projectionMatrix(Math::toRadian(this->_fov), this->_far, this->_near, glfw._width, glfw._height);
	this->_viewMatrix.viewMatrix(this->local._pos, this->local._rot);
}
Cam::Cam(Glfw& glfw, float posX, float posY, float posZ, float rotX, float rotY, float rotZ) {//degree
	cout << "_ Cam Cons (xyz pos, xyz rot)" << endl;
	this->local._pos.x = posX;
	this->local._pos.y = posY;
	this->local._pos.z = posZ;
	this->local._rot.x = rotX;
	this->local._rot.y = rotY;
	this->local._rot.z = rotZ;
	this->local._rot.setUnit(ROT_DEG);
	this->_fov = CAM_FOV;
	this->_near = CAM_NEAR;
	this->_far = CAM_FAR;
	this->updateCamVectors();
	this->_projectionMatrix.projectionMatrix(Math::toRadian(this->_fov), this->_far, this->_near, glfw._width, glfw._height);
	this->_viewMatrix.viewMatrix(this->local._pos, this->local._rot);
}

Cam::~Cam() {}

void	Cam::printProperties() const {
	cout << "pos:\t" << this->local._pos.x << " " << this->local._pos.y << " " << this->local._pos.z << endl;
	cout << "rot:\t" << this->local._rot.x << " " << this->local._rot.y << " " << this->local._rot.z << endl;
	cout << "Far:\t" << this->_far << endl;
	cout << "Near:\t" << this->_near << endl;
	cout << "FOV:\t" << this->_fov << endl;
}

void	Cam::updateCamVectors(void) {
	this->_right = Math::Vector3(1, 0, 0);
	this->_right.ZYXrotate(this->local._rot, -ROT_WAY);
	this->_up = Math::Vector3(0, 1, 0);
	this->_up.ZYXrotate(this->local._rot, -ROT_WAY);
	this->_forward = Math::Vector3::cross(this->_up, this->_right);
}

void	Cam::updateViewMatrix() {
	this->update();
	if (this->local._matrixChanged) {
		//care of scale, must be 1,1,1 or matrix will be wrong
		if (this->local._scale.x == 1.0f && \
			this->local._scale.y == 1.0f && \
			this->local._scale.z == 1.0f) {
			this->_viewMatrix = this->local._matrix;
		}
		else
			this->_viewMatrix.viewMatrix(this->local._pos, this->local._rot);
		Math::Rotation	camRot = this->local._rot;
		Math::Vector3	camPos = this->local._pos;
		camRot.setAsRad();
		camPos.rotate(camRot, ROT_WAY);
		this->_viewMatrix.updatePosValue(-camPos);
	}
}

void	Cam::events(Glfw& glfw, float fpsTick) {
	this->local._rot.setUnit(ROT_DEG);
	this->local._rot.z = 0;
	this->local._rot.y = -(float(glfw._mouseDiffX) * 360.0f / MOUSE_SENSIBILITY);//mouse's X pos for cam's Y rot axis
	this->local._rot.x = -(float(glfw._mouseDiffY) * 360.0f / MOUSE_SENSIBILITY);//mouse's Y pos for cam's X rot axis
	this->updateCamVectors();

	float	movement = CAM_SPEED * fpsTick;
	Math::Vector3	mvt[3];
	mvt[0] = this->_forward;
	mvt[1] = this->_right;
	mvt[2] = this->_up;
	mvt[0].mult(movement);
	mvt[1].mult(movement);
	mvt[2].mult(movement);
	if (GLFW_PRESS == glfwGetKey(glfw._window, GLFW_KEY_W))
		this->local._pos.add(mvt[0]);
	if (GLFW_PRESS == glfwGetKey(glfw._window, GLFW_KEY_S))
		this->local._pos.sub(mvt[0]);
	if (GLFW_PRESS == glfwGetKey(glfw._window, GLFW_KEY_D))
		this->local._pos.add(mvt[1]);
	if (GLFW_PRESS == glfwGetKey(glfw._window, GLFW_KEY_A))
		this->local._pos.sub(mvt[1]);
	if (GLFW_PRESS == glfwGetKey(glfw._window, GLFW_KEY_R))
		this->local._pos.add(mvt[2]);
	if (GLFW_PRESS == glfwGetKey(glfw._window, GLFW_KEY_F))
		this->local._pos.sub(mvt[2]);

	this->_viewMatrix.viewMatrix(this->local._pos, this->local._rot);
	if (GLFW_PRESS == glfwGetKey(glfw._window, GLFW_KEY_M))
		this->_viewMatrix.printData();
}

//mutator // a completer (update projection matrix)
void	Cam::setProjectionMatrix(Math::Matrix4& projection) {
	this->_projectionMatrix = projection;
}
void	Cam::setViewMatrix(Math::Matrix4& view) {
	this->_viewMatrix = view;
}

void	Cam::setFov(float fov) {
	fov = min(fov, CAM_FOV_MAX);
	this->_fov = max(fov, CAM_FOV_MIN);
	//update projectionMatrix
}
void	Cam::setNear(float nearValue) {
	nearValue = min(nearValue, CAM_NEAR_MAX);
	this->_near = max(nearValue, CAM_NEAR_MIN);
	//update projectionMatrix
}
void	Cam::setFar(float farValue) {
	farValue = min(farValue, CAM_FAR_MAX);
	this->_far = max(farValue, CAM_FAR_MIN);
	//update projectionMatrix
}
//accessor
Math::Matrix4&	Cam::getProjectionMatrix() const { return ((Math::Matrix4&)this->_projectionMatrix); }
Math::Matrix4&	Cam::getViewMatrix() const { return ((Math::Matrix4&)this->_viewMatrix); }
float			Cam::getFov() const { return (this->_fov); }
float			Cam::getNear() const { return (this->_near); }
float			Cam::getFar() const { return (this->_far); }
