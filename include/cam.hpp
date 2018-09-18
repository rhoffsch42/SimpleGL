#pragma once
#include "math.hpp"
#include "glfw.hpp"

//camera settings
#define CAM_FOV			90.0f
#define CAM_FOV_MIN		60.0f
#define CAM_FOV_MAX		150.0f
#define CAM_FAR			2000.0f
#define CAM_FAR_MIN		10.0f
#define CAM_FAR_MAX		3000.0f
#define CAM_NEAR		0.1f
#define CAM_NEAR_MIN	0.1f
#define CAM_NEAR_MAX	5.0f

//window settings
#define DEF_WIN_X	800
#define DEF_WIN_Y	800
#define MAX_WIN_X	1920
#define MAX_WIN_Y	1080
#define MIN_WIN_X	800
#define MIN_WIN_Y	800

class Glfw;

class Cam
{
public:
	Cam(Glfw& glfw);
	Cam(Glfw& glfw, Math::Vector3 pos, Math::Rotation rot);
	Cam(Glfw& glfw, float posX, float posY, float posZ, float rotX, float rotY, float rotZ);
	~Cam();

	void	printProperties() const;
	void	updateCamVectors();
	void	events(Glfw& glfw, float fpsTick);
	//mutator
	void	setProjectionMatrix(Math::Matrix4 projection);
	void	setViewMatrix(Math::Matrix4 view);
	void	setPos(Math::Vector3 pos);
	void	setPos(float x, float y, float z);
	void	setRot(Math::Rotation rot);
	void	setRot(float x, float y, float z);//degree
	void	setFov(float fov);
	void	setNear(float nearValue);
	void	setFar(float farValue);
	//accessor
	Math::Matrix4&	getProjectionMatrix() const;
	Math::Matrix4&	getViewMatrix() const;
	Math::Vector3	getPos() const;
	Math::Rotation	getRot() const;
	float			getFov() const;
	float			getNear() const;
	float			getFar() const;
private:
	Math::Matrix4	_projectionMatrix;
	Math::Matrix4	_viewMatrix;
	Math::Vector3	_pos;
	Math::Rotation	_rot;
	Math::Vector3	_right;
	Math::Vector3	_up;
	Math::Vector3	_forward;
	float			_fov;
	float			_near;
	float			_far;
	//int			winX, winY;
};