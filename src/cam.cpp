#include "cam.hpp"
#include "simplegl.h"
#include "compiler_settings.h"

#ifdef SGL_DEBUG
 #define SGL_CAM_DEBUG
#endif
#ifdef SGL_CAM_DEBUG 
 #define D(x) std::cout << "[Cam] " << x ;
 #define D_(x) x
 #define D_SPACER "-- cam.cpp -------------------------------------------------\n"
 #define D_SPACER_END "----------------------------------------------------------------\n"
#else
 #define D(x)
 #define D_(x)
 #define D_SPACER ""
 #define D_SPACER_END ""
#endif

void	Cam::init(int width, int height) {
	this->_fov = CAM_FOV;
	this->_near = CAM_NEAR;
	this->_far = CAM_FAR;
	this->_aspectRatio = float(width) / float(height);
	this->speed = CAM_SPEED;
	this->lockedMovement = false;
	this->lockedOrientation = false;
	this->space_movement = false;
	this->updateCamVectors();
	this->_projectionMatrix.projectionMatrix(Math::toRadian(this->_fov), this->_far, this->_near, width, height);
	this->updateFrustum();
	this->_viewMatrix.viewMatrix(this->local._pos, this->local._rot);
}

Cam::Cam(int width, int height) : Object() {
	D(__PRETTY_FUNCTION__ << std::endl)
	this->init(width, height);
}
Cam::Cam(int width, int height, Math::Vector3 pos, Math::Rotation rot) : Object() {
	D("_ " << __PRETTY_FUNCTION__ << std::endl)
	this->local._pos = pos;
	this->local._rot = rot;
	this->init(width, height);
}
Cam::Cam(int width, int height, float posX, float posY, float posZ, float rotX, float rotY, float rotZ) {//degree
	D("_ " << __PRETTY_FUNCTION__ << std::endl)
	this->local._pos.x = posX;
	this->local._pos.y = posY;
	this->local._pos.z = posZ;
	this->local._rot.x = rotX;
	this->local._rot.y = rotY;
	this->local._rot.z = rotZ;
	this->local._rot.setUnit(ROT_DEG);
	this->init(width, height);
}

Cam::~Cam() {}

void	Cam::printProperties() const {
	D("Camera settings: " << this << "\t&ViewMatrix: " << &(this->_viewMatrix) << std::endl)
	D("pos:\t" << this->local._pos.x << " " << this->local._pos.y << " " << this->local._pos.z << std::endl)
	D("rot:\t" << this->local._rot.x << " " << this->local._rot.y << " " << this->local._rot.z << std::endl)
	D("Far:\t" << this->_far << std::endl)
	D("Near:\t" << this->_near << std::endl)
	D("FOV:\t" << this->_fov << std::endl)
	D("ratio:\t" << this->_aspectRatio << std::endl)
	D("-----------" << std::endl)
}

void	Cam::updateCamVectors(void) {
	this->_right = Math::Vector3(1, 0, 0);
	this->_right.ZYXrotate(this->local._rot, -ROT_WAY);
	this->_up = Math::Vector3(0, 1, 0);
	this->_up.ZYXrotate(this->local._rot, -ROT_WAY);
	this->_forward = Math::Vector3::cross(this->_up, this->_right);
}

void	Cam::updateFrustum() {//make a switch case to update the corresponding values?
	//from http://www.iquilezles.org/www/articles/frustum/frustum.htm iqFrustumF_CreatePerspective
	const float an = this->_fov / 2.0f * (3.141592653589f / 180.0f);//corrected
	const float si = sinf(an);
	const float co = cosf(an);
	/*
	_frustum[0] = 0.0f;		_frustum[1] = -co;		_frustum[2] = si;					_frustum[3] = 0.0f;//top
	_frustum[4] = 0.0f;		_frustum[5] = co;		_frustum[6] = si;					_frustum[7] = 0.0f;//bottom
	_frustum[8] = co;		_frustum[9] = 0.0f;		_frustum[10] = si * _aspectRatio;	_frustum[11] = 0.0f;//left
	_frustum[12] = -co;		_frustum[13] = 0.0f;	_frustum[14] = si * _aspectRatio;	_frustum[15] = 0.0f;//right
	_frustum[16] = 0.0f;	_frustum[17] = 0.0f;	_frustum[18] = 1.0f;				_frustum[19] = _far;//far
	_frustum[20] = 0.0f;	_frustum[21] = 0.0f;	_frustum[22] = -1.0f;				_frustum[23] = -_near;//near
	*/

	//corrected signs
	_frustum[0] = 0.0f;		_frustum[1] = -co;		_frustum[2] = -si;					_frustum[3] = 0.0f;//top
	_frustum[4] = 0.0f;		_frustum[5] = co;		_frustum[6] = -si;					_frustum[7] = 0.0f;//bottom
	_frustum[8] = co;		_frustum[9] = 0.0f;		_frustum[10] = -si * _aspectRatio;	_frustum[11] = 0.0f;//left
	_frustum[12] = -co;		_frustum[13] = 0.0f;	_frustum[14] = -si * _aspectRatio;	_frustum[15] = 0.0f;//right
	_frustum[16] = 0.0f;	_frustum[17] = 0.0f;	_frustum[18] = 1.0f;				_frustum[19] = _far;//far
	_frustum[20] = 0.0f;	_frustum[21] = 0.0f;	_frustum[22] = -1.0f;				_frustum[23] = -_near;//near
}

//target_pos must be in cam space
bool	Cam::isInFrustum(Math::Vector3 target_pos, Math::Matrix4 VPmatrix) {
	target_pos.mult(VPmatrix);// transform object pos to camera space
	int	in = 0;
	size_t	max = 4;// 6 = all planes, 4 = no far no near
	for (size_t i = 0; i < max; i++) {
		Math::Vector3	normal(_frustum[i * 4 + 0], _frustum[i * 4 + 1], _frustum[i * 4 + 2]);
		if (Math::Vector3::dot(normal, target_pos) > 0)
			in++;
	}
	
	if (max == 4) {//manual for near and far
		max += 2;
		if (target_pos.z > -_frustum[19])
			in++;
		if (target_pos.z < _frustum[23])
			in++;
	}
	return (in == max);//is on the right halfspace of all planes, ie. is in the frustum
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
			//D("viewMatrix with parents (if there are) - only POS" << std::endl)
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
			//D("viewMatrix without parents" << std::endl)
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
		if (this->space_movement) {
			this->_mvt[2] = this->_up;
		} else {
			this->_mvt[0].y = 0;
			this->_mvt[0].normalize();
			this->_mvt[1].y = 0;
			this->_mvt[1].normalize();
			this->_mvt[2] = Math::Vector3(0, 1, 0);
		}
		this->_mvt[0].mult(effectiveSpeed);
		this->_mvt[1].mult(effectiveSpeed);
		this->_mvt[2].mult(effectiveSpeed);

		if (GLFW_PRESS == glfwGetKey(glfw._window, GLFW_KEY_W))
			this->local._pos.add(this->_mvt[0]);
		if (GLFW_PRESS == glfwGetKey(glfw._window, GLFW_KEY_S))
			this->local._pos.sub(this->_mvt[0]);
		if (GLFW_PRESS == glfwGetKey(glfw._window, GLFW_KEY_D))
			this->local._pos.add(this->_mvt[1]);
		if (GLFW_PRESS == glfwGetKey(glfw._window, GLFW_KEY_A))
			this->local._pos.sub(this->_mvt[1]);
		if (GLFW_PRESS == glfwGetKey(glfw._window, GLFW_KEY_SPACE))
			this->local._pos.add(this->_mvt[2]);
		if (GLFW_PRESS == glfwGetKey(glfw._window, GLFW_KEY_C))
			this->local._pos.sub(this->_mvt[2]);
	}

	this->updateViewMatrix();
	if (GLFW_PRESS == glfwGetKey(glfw._window, GLFW_KEY_M))
		this->_viewMatrix.printData();
}

//mutator

//fov, far, near, and aspectRatio may not corespond anymore
void	Cam::setProjectionMatrix(Math::Matrix4& projection) {
	this->_projectionMatrix = projection;
}
void	Cam::setViewMatrix(Math::Matrix4& view) {
	this->_viewMatrix = view;
}

//update the projection matrix and the frustum accordingly
void	Cam::setFov(float fov) {
	fov = std::min(fov, CAM_FOV_MAX);
	this->_fov = std::max(fov, CAM_FOV_MIN);
	this->_projectionMatrix.projectionMatrix(Math::toRadian(this->_fov), this->_far, this->_near, this->_aspectRatio, 1.0f);
	this->updateFrustum();//could update only the right values
}
//update the projection matrix and the frustum accordingly
void	Cam::setNear(float nearValue) {
	nearValue = std::min(nearValue, CAM_NEAR_MAX);
	this->_near = std::max(nearValue, CAM_NEAR_MIN);
	this->_projectionMatrix.projectionMatrix(Math::toRadian(this->_fov), this->_far, this->_near, this->_aspectRatio, 1.0f);
	this->updateFrustum();//could update only the right value
}
//update the projection matrix and the frustum accordingly
void	Cam::setFar(float farValue) {
	farValue = std::min(farValue, CAM_FAR_MAX);
	this->_far = std::max(farValue, CAM_FAR_MIN);
	this->_projectionMatrix.projectionMatrix(Math::toRadian(this->_fov), this->_far, this->_near, this->_aspectRatio, 1.0f);
	this->updateFrustum();//could update only the right value
}

//accessor

Math::Matrix4&	Cam::getProjectionMatrix() const { return ((Math::Matrix4&)this->_projectionMatrix); }
Math::Matrix4&	Cam::getViewMatrix() const { return ((Math::Matrix4&)this->_viewMatrix); }
float			Cam::getFov() const { return (this->_fov); }
float			Cam::getNear() const { return (this->_near); }
float			Cam::getFar() const { return (this->_far); }
float			Cam::getAspectRatio() const { return (this->_aspectRatio); }
