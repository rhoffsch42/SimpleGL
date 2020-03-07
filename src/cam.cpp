#include "simplegl.h"
#include "cam.hpp"

Cam::Cam(Glfw& glfw) : Object() {
	std::cout << "_ " << __PRETTY_FUNCTION__ << std::endl;
	this->_fov = CAM_FOV;
	this->_near = CAM_NEAR;
	this->_far = CAM_FAR;
	this->speed = CAM_SPEED;
	this->lockedMovement = false;
	this->lockedOrientation = false;
	this->updateCamVectors();
	this->_projectionMatrix.projectionMatrix(Math::toRadian(this->_fov), this->_far, this->_near, glfw._width, glfw._height);
	this->_viewMatrix.viewMatrix(this->local._pos, this->local._rot);
}
Cam::Cam(Glfw& glfw, Math::Vector3 pos, Math::Rotation rot) : Object() {
	std::cout << "_ " << __PRETTY_FUNCTION__ << std::endl;
	this->_fov = CAM_FOV;
	this->_near = CAM_NEAR;
	this->_far = CAM_FAR;
	this->speed = CAM_SPEED;
	this->lockedMovement = false;
	this->lockedOrientation = false;
	this->local._pos = pos;
	this->local._rot = rot;
	this->updateCamVectors();
	this->_projectionMatrix.projectionMatrix(Math::toRadian(this->_fov), this->_far, this->_near, glfw._width, glfw._height);
	this->_viewMatrix.viewMatrix(this->local._pos, this->local._rot);
}
Cam::Cam(Glfw& glfw, float posX, float posY, float posZ, float rotX, float rotY, float rotZ) {//degree
	std::cout << "_ " << __PRETTY_FUNCTION__ << std::endl;
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
	this->speed = CAM_SPEED;
	this->lockedMovement = false;
	this->lockedOrientation = false;
	this->updateCamVectors();
	this->_projectionMatrix.projectionMatrix(Math::toRadian(this->_fov), this->_far, this->_near, glfw._width, glfw._height);
	this->_viewMatrix.viewMatrix(this->local._pos, this->local._rot);

}

Cam::~Cam() {}

void	Cam::printProperties() const {
	cout << "Camera settings: " << this << "\t&ViewMatrix: " << &(this->_viewMatrix) << endl;
	cout << "pos:\t" << this->local._pos.x << " " << this->local._pos.y << " " << this->local._pos.z << endl;
	cout << "rot:\t" << this->local._rot.x << " " << this->local._rot.y << " " << this->local._rot.z << endl;
	cout << "Far:\t" << this->_far << endl;
	cout << "Near:\t" << this->_near << endl;
	cout << "FOV:\t" << this->_fov << endl;
	cout << "-----------" << endl;
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
	if (this->_worldMatrixChanged) {//for now this is always true, cf Cam::events (this->local._matrixUpdated = false;)
		// check parent's matrix changed too ?
		/*
			worldMatrix's scale must be 1,1,1 or viewmatrix will be affected in a bad way,
			undefined behavior if we don't check
			
			rescale rotation part of the worldMatrix to a 1,1,1 scale
			extract euler angles and pos from worldMatrix, then:
			this->_viewMatrix.viewMatrix(pos, rot);
		*/
		if (true) {
			// cout << "viewMatrix with parents (if there are) - only POS" << endl;
			t_pp	pp = Math::extractFromMatrix(this->_worldMatrix);
			/*
				update local ? depending on the parent
				need world to local matrix, this is the inverse matrix of the parent ?
					Parent * Local = Word
			*/
			this->_viewMatrix.viewMatrix(pp.pos, pp.rot);
			this->_viewMatrix.setOrder(COLUMN_MAJOR);
			this->_worldMatrixChanged = false;// is this needed ? for now updateViewMatrix is done at each frame
			// this->_viewMatrix.printData();
		} else {
			// cout << "viewMatrix without parents" << endl;
			this->_viewMatrix.viewMatrix(this->local._pos, this->local._rot);// this does not take parents into account
		}
	}
}

void	Cam::events(Glfw& glfw, float fpsTick) {
	/*
		as long as this is done at every frame, the viewmatrix has to be updated.
		Need to know if there are changes with camera events (mouse + wasd), to avoid useless calculations
			-> set a bool cameraMoved; // pos + rot, + parent world changed
		glfwWaitEvents
			http://www.glfw.org/docs/latest/group__window.html#ga554e37d781f0a997656c26b2c56c835e
		void cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
			/!\ need global Glfw to access mouse variables
	*/


	if (!this->lockedOrientation) {
		this->local._rot.setUnit(ROT_DEG);
		this->local._rot.z = 0;
		this->local._rot.y = (float(glfw._mouseDiffX) * 360.0f / MOUSE_SENSIBILITY);//mouse's X pos for cam's Y rot axis
		this->local._rot.x = (float(glfw._mouseDiffY) * 360.0f / MOUSE_SENSIBILITY);//mouse's Y pos for cam's X rot axis
		this->local._matrixUpdated = false;
		this->updateCamVectors();
	}
	if (!this->lockedMovement) {
		//this should be done only once too
		float	effectiveSpeed = this->speed * fpsTick;
		this->_mvt[0] = this->_forward;
		this->_mvt[1] = this->_right;
		this->_mvt[2] = this->_up;
		this->_mvt[0].mult(effectiveSpeed);
		this->_mvt[1].mult(effectiveSpeed);
		this->_mvt[2].mult(effectiveSpeed);
		//
		if (GLFW_PRESS == glfwGetKey(glfw._window, GLFW_KEY_W))
			this->local._pos.add(this->_mvt[0]);
		if (GLFW_PRESS == glfwGetKey(glfw._window, GLFW_KEY_S))
			this->local._pos.sub(this->_mvt[0]);
		if (GLFW_PRESS == glfwGetKey(glfw._window, GLFW_KEY_D))
			this->local._pos.add(this->_mvt[1]);
		if (GLFW_PRESS == glfwGetKey(glfw._window, GLFW_KEY_A))
			this->local._pos.sub(this->_mvt[1]);
		if (GLFW_PRESS == glfwGetKey(glfw._window, GLFW_KEY_R))
			this->local._pos.add(this->_mvt[2]);
		if (GLFW_PRESS == glfwGetKey(glfw._window, GLFW_KEY_F))
			this->local._pos.sub(this->_mvt[2]);
	}

	this->updateViewMatrix();
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
