#pragma once

#include "obj3dPG.hpp"
#include "skyboxPG.hpp"
#include "framebuffer.hpp"
#include "uipanel.hpp"
#include "obj3d.hpp"
#include "skybox.hpp"
#include "math.hpp"

#define WINX 1600
#define WINY 900

class Renderer
{
public:
	static void blitToWindow(FrameBuffer* readFramebuffer, GLenum attachmentPoint, UIPanel* panel);

	//cubemap is for skybox
	Renderer(std::string mesh_vs, std::string mesh_fs, std::string cubemap_vs, std::string cubemap_fs);
	~Renderer();

	void	renderObj3d(list<Obj3d*>& obj3dList, Cam& cam, bool force_draw = false);
	void	renderSkybox(Skybox& skybox, Cam& cam);

	Obj3dPG*	meshProgram;
	SkyboxPG*	skyboxProgram;
private:
	float	isForward(Properties& local_referenciel, Math::Vector3 target_pos);//in Properties:: ?
	bool	isInFrustum(float* frustum, Math::Vector3 target_pos);//in Cam:: ?
	void	iqFrustumF_CreatePerspective(float* frus, float fovy, float aspect, float znear, float zfar);//in Cam:: ?

};