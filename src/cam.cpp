#include "simplegl.h"
#include "cam.hpp"

Cam::Cam(Glfw& glfw) {
	cout << "_ Cam Cons default" << endl;
	this->_fov = CAM_FOV;
	this->_near = CAM_NEAR;
	this->_far = CAM_FAR;
	this->updateCamVectors();
	this->_projectionMatrix.projectionMatrix(Math::toRadian(this->_fov), this->_far, this->_near, glfw._width, glfw._height);
	this->_viewMatrix.viewMatrix(this->_pos, this->_rot);
}
Cam::Cam(Glfw& glfw, Math::Vector3 pos, Math::Rotation rot) : _pos(pos), _rot(rot) {
	cout << "_ Cam Cons (pos, rot)" << endl;
	this->_fov = CAM_FOV;
	this->_near = CAM_NEAR;
	this->_far = CAM_FAR;
	this->updateCamVectors();
	this->_projectionMatrix.projectionMatrix(Math::toRadian(this->_fov), this->_far, this->_near, glfw._width, glfw._height);
	this->_viewMatrix.viewMatrix(this->_pos, this->_rot);
}
Cam::Cam(Glfw& glfw, float posX, float posY, float posZ, float rotX, float rotY, float rotZ) {
	cout << "_ Cam Cons (xyz pos, xyz rot)" << endl;
	this->_pos.x = posX;
	this->_pos.y = posY;
	this->_pos.z = posZ;
	this->_rot.x = rotX;
	this->_rot.y = rotY;
	this->_rot.z = rotZ;
	this->_fov = CAM_FOV;
	this->_near = CAM_NEAR;
	this->_far = CAM_FAR;
	this->updateCamVectors();
	this->_projectionMatrix.projectionMatrix(Math::toRadian(this->_fov), this->_far, this->_near, glfw._width, glfw._height);
	this->_viewMatrix.viewMatrix(this->_pos, this->_rot);
}

Cam::~Cam() {}

void	Cam::printProperties() const {
	cout << "pos:\t" << this->_pos.x << " " << this->_pos.y << " " << this->_pos.z << endl;
	cout << "rot:\t" << this->_rot.x << " " << this->_rot.y << " " << this->_rot.z << endl;
	cout << "Far:\t" << this->_far << endl;
	cout << "Near:\t" << this->_near << endl;
	cout << "FOV:\t" << this->_fov << endl;
}

void	Cam::updateCamVectors(void) {
	this->_right = Math::Vector3(1, 0, 0);
	this->_right.ZYXrotate(this->_rot, -ROT_WAY);
	this->_up = Math::Vector3(0, 1, 0);
	this->_up.ZYXrotate(this->_rot, -ROT_WAY);
	this->_forward = Math::Vector3::cross(this->_up, this->_right);
}

void	Cam::events(Glfw& glfw, float fpsTick) {
	this->_rot.setUnit(ROT_DEG);
	this->_rot.z = 0;
	this->_rot.y = -(float(glfw._mouseDiffX) * 360.0f / MOUSE_SENSIBILITY);//mouse's X pos for cam's Y rot axis
	this->_rot.x = -(float(glfw._mouseDiffY) * 360.0f / MOUSE_SENSIBILITY);//mouse's Y pos for cam's X rot axis
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
		this->_pos.add(mvt[0]);
	if (GLFW_PRESS == glfwGetKey(glfw._window, GLFW_KEY_S))
		this->_pos.sub(mvt[0]);
	if (GLFW_PRESS == glfwGetKey(glfw._window, GLFW_KEY_D))
		this->_pos.add(mvt[1]);
	if (GLFW_PRESS == glfwGetKey(glfw._window, GLFW_KEY_A))
		this->_pos.sub(mvt[1]);
	if (GLFW_PRESS == glfwGetKey(glfw._window, GLFW_KEY_R))
		this->_pos.add(mvt[2]);
	if (GLFW_PRESS == glfwGetKey(glfw._window, GLFW_KEY_F))
		this->_pos.sub(mvt[2]);

	this->_viewMatrix.viewMatrix(this->_pos, this->_rot);
	if (GLFW_PRESS == glfwGetKey(glfw._window, GLFW_KEY_M))
		this->_viewMatrix.printData();
}

//mutator // a completer (update projection matrix)
void	Cam::setProjectionMatrix(Math::Matrix4 projection) {
	this->_projectionMatrix = projection;
}
void	Cam::setViewMatrix(Math::Matrix4 view) {
	this->_viewMatrix = view;
}
void	Cam::setPos(Math::Vector3 pos) {
	this->_pos = pos;
	//negative pos, cf Matrix4::viewMatrix() and Matrix4::updatePosValue()
	this->_viewMatrix.updatePosValue(-this->_pos);
}
void	Cam::setPos(float x, float y, float z) {
	this->_pos.x = x;
	this->_pos.y = y;
	this->_pos.z = z;
	//negative pos, cf Matrix4::viewMatrix() and Matrix4::updatePosValue()
	this->_viewMatrix.updatePosValue(-this->_pos);
}
void	Cam::setRot(Math::Rotation rot) {
	this->_rot = rot;
	this->updateCamVectors();
	this->_viewMatrix.viewMatrix(this->_pos, this->_rot);// pas opti, peut etre fait plusieurs fois par frame
}
void	Cam::setRot(float x, float y, float z) {//degree
	this->_rot.x = x;
	this->_rot.y = y;
	this->_rot.z = z;
	this->_rot.setUnit(ROT_DEG);
	this->updateCamVectors();
	this->_viewMatrix.viewMatrix(this->_pos, this->_rot);// pas opti, peut etre fait plusieurs fois par frame
}
void	Cam::setFov(float fov) {
	fov = min(fov, CAM_FOV_MAX);
	this->_fov = max(fov, CAM_FOV_MIN);
}
void	Cam::setNear(float nearValue) {
	nearValue = min(nearValue, CAM_NEAR_MAX);
	this->_near = max(nearValue, CAM_NEAR_MIN);
}
void	Cam::setFar(float farValue) {
	farValue = min(farValue, CAM_FAR_MAX);
	this->_far = max(farValue, CAM_FAR_MIN);
}
//accessor
Math::Matrix4&	Cam::getProjectionMatrix() const { return ((Math::Matrix4&)this->_projectionMatrix); }
Math::Matrix4&	Cam::getViewMatrix() const { return ((Math::Matrix4&)this->_viewMatrix); }
Math::Vector3	Cam::getPos() const { return (this->_pos); }
Math::Rotation	Cam::getRot() const { return (this->_rot); }
float			Cam::getFov() const { return (this->_fov); }
float			Cam::getNear() const { return (this->_near); }
float			Cam::getFar() const { return (this->_far); }
