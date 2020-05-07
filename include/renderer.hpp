#pragma once

#include "framebuffer.hpp"
#include "uipanel.hpp"

#include "program.hpp"
#include "obj3dPG.hpp"
#include "skyboxPG.hpp"

#include "object.hpp"
#include "obj3d.hpp"
#include "skybox.hpp"

#include "math.hpp"

#include <list>

#define WINX 1600
#define WINY 900

class Renderer
{
public:
	static void blitToWindow(FrameBuffer* readFramebuffer, GLenum attachmentPoint, UIPanel* panel);

	//cubemap is for skybox
	virtual ~Renderer();

	//if the rendered object are not Object::  we will have to let the child class declare it without declaring that virtual pure here
	virtual void	renderObjects(list<Object*>& list, Cam& cam, bool force_draw = false) = 0;

	Program*	program;
protected:
	Renderer(std::string vertexShader, std::string fragmentShader);

	float	isForward(Properties& local_referenciel, Math::Vector3 target_pos);//in Properties:: ?
	bool	isInFrustum(float* frustum, Math::Vector3 target_pos);//in Cam:: ?
	void	iqFrustumF_CreatePerspective(float* frus, float fovy, float aspect, float znear, float zfar);//in Cam:: ?
private:
};

class MeshRenderer : public Renderer
{
public:
	MeshRenderer(std::string vertexShader, std::string fragmentShader);
	virtual ~MeshRenderer();

	void	renderObjects(list<Object*>& list, Cam& cam, bool force_draw = false);

	Obj3dPG*	program;//check what happen in the destructor, does it call the right ones in the correct order?
private:
};


class SkyboxRenderer : public Renderer
{
public:
	SkyboxRenderer(std::string vertexShader, std::string fragmentShader);
	virtual ~SkyboxRenderer();

	void	renderObjects(list<Object*>& list, Cam& cam, bool force_draw = false);

	SkyboxPG* program;//check what happen in the destructor, does it call the right ones in the correct order?
private:
};