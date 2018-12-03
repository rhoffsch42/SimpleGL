#include "simplegl.h"
#include "cam.hpp"

Cam::Cam(Glfw& glfw) : Object() {
	cout << "_ Cam Cons default" << endl;
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
	cout << "_ Cam Cons (pos, rot)" << endl;
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
	this->speed = CAM_SPEED;
	this->lockedMovement = false;
	this->lockedOrientation = false;
	this->updateCamVectors();
	this->_projectionMatrix.projectionMatrix(Math::toRadian(this->_fov), this->_far, this->_near, glfw._width, glfw._height);
	this->_viewMatrix.viewMatrix(this->local._pos, this->local._rot);
}

Cam::~Cam() {}

void	Cam::printProperties() const {
	cout << "Camera settings:" << endl;
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
	this->update();//this is for Object in general, but cam matrix works differently
	/*
	what we want:
		cam updated pos and rot, scale must be 1,1,1
			if the matrix is generated with viewMatrix(...) the scale is ignored
		how to get new pos:
			extract it from the parent's worldMatrix	m[xyz][3]
		how to get new rot:
			extract it from the parent's worldMatrix, but how?
			the xyz rot are encoded in the 3x3matrix with the scale
			is the magnitude of the vector m[][] the scale of the parent object? yes apparently
	*/
/*
	if (this->_parent) {
		this->_parent->update();
		Math::Matrix4	mat(this->_parent->getWorldMatrix());
		mat.setOrder(ROW_MAJOR);
		float	(&m)[4][4] = *reinterpret_cast<float(*)[4][4]>(mat.getData());
		this->local._pos.x = m[0][3];
		this->local._pos.y = m[1][3];
		this->local._pos.z = m[2][3];
		// this->local._rot = this->_parent->local.getRot();
		this->updateCamVectors();
		Math::Vector3	vec3 = this->_forward;
		vec3.mult(-30.0f);
		this->local.translate(vec3);
		this->_worldMatrixChanged = true;
	}
*/
	if (this->_worldMatrixChanged) {//for now this is always true, cf Cam::events (this->local._matrixUpdated = false;)
		/*
			worldMatrix's scale must be 1,1,1 or viewmatrix will be affected in a bad way, undefined behavior if we don't check
			
			extract euler angles and pos from worldMatrix, then:
			this->_viewMatrix.viewMatrix(pos, rot);
		*/
		if (1) {
			cout << "viewMatrix with parents (if there are) - only POS" << endl;
			Math::Vector3 pos;
			this->_worldMatrix.setOrder(ROW_MAJOR);
			pos.x = this->_worldMatrix.tab[0][3];
			pos.y = this->_worldMatrix.tab[1][3];
			pos.z = this->_worldMatrix.tab[2][3];
			this->_viewMatrix.viewMatrix(pos, this->local._rot);

			if (0) {//rescale to 1,1,1
				Math::Vector3	vectorX;
				Math::Vector3	vectorY;
				Math::Vector3	vectorZ;
				vectorX.x = this->_worldMatrix.tab[0][0];
				vectorX.y = this->_worldMatrix.tab[1][0];
				vectorX.z = this->_worldMatrix.tab[2][0];
				vectorY.x = this->_worldMatrix.tab[0][1];
				vectorY.y = this->_worldMatrix.tab[1][1];
				vectorY.z = this->_worldMatrix.tab[2][1];
				vectorZ.x = this->_worldMatrix.tab[0][2];
				vectorZ.y = this->_worldMatrix.tab[1][2];
				vectorZ.z = this->_worldMatrix.tab[2][2];
				Math::Vector3 scale;
				scale.x = vectorX.magnitude();
				scale.y = vectorY.magnitude();
				scale.z = vectorZ.magnitude();
				scale.printData();
				this->_viewMatrix.setOrder(ROW_MAJOR);
				this->_viewMatrix.tab[0][0] = this->_worldMatrix.tab[0][0] / scale.x;
				this->_viewMatrix.tab[1][0] = this->_worldMatrix.tab[1][0] / scale.x;
				this->_viewMatrix.tab[2][0] = this->_worldMatrix.tab[2][0] / scale.x;
				this->_viewMatrix.tab[0][1] = this->_worldMatrix.tab[0][1] / scale.y;
				this->_viewMatrix.tab[1][1] = this->_worldMatrix.tab[1][1] / scale.y;
				this->_viewMatrix.tab[2][1] = this->_worldMatrix.tab[2][1] / scale.y;
				this->_viewMatrix.tab[0][2] = this->_worldMatrix.tab[0][2] / scale.z;
				this->_viewMatrix.tab[1][2] = this->_worldMatrix.tab[1][2] / scale.z;
				this->_viewMatrix.tab[2][2] = this->_worldMatrix.tab[2][2] / scale.z;
				// matrix pos vector isnt correct, need to extract euler angles from that
				// then: this->_viewMatrix.viewMatrix(pos, eulerAngles);
				this->_viewMatrix.setOrder(COLUMN_MAJOR);
			}
			this->_viewMatrix.printData();
		} else {
			cout << "viewMatrix without parents" << endl;
			this->_viewMatrix.viewMatrix(this->local._pos, this->local._rot);// this does not take parents into account
		}
	}
}

void	Cam::events(Glfw& glfw, float fpsTick) {
	/*
		as long as this is done at every frame, the viewmatrix has to be updated.
		Need to know if there are changes with mouse events, to avoid useless calculations
		glfwWaitEvents
			http://www.glfw.org/docs/latest/group__window.html#ga554e37d781f0a997656c26b2c56c835e
		void cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
			/!\ need global Glfw to access mouse variables
	*/
	if (!this->lockedOrientation) {
		this->local._rot.setUnit(ROT_DEG);
		this->local._rot.z = 0;
		this->local._rot.y = -(float(glfw._mouseDiffX) * 360.0f / MOUSE_SENSIBILITY);//mouse's X pos for cam's Y rot axis
		this->local._rot.x = -(float(glfw._mouseDiffY) * 360.0f / MOUSE_SENSIBILITY);//mouse's Y pos for cam's X rot axis
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
