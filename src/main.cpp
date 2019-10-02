/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rhoffsch <rhoffsch@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/18 22:45:30 by rhoffsch          #+#    #+#             */
/*   Updated: 2019/04/03 14:58:50 by rhoffsch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "simplegl.h"

#include "program.hpp"
#include "object.hpp"
#include "obj3dPG.hpp"
#include "obj3dBP.hpp"
#include "obj3d.hpp"
#include "misc.hpp"
#include "cam.hpp"
#include "texture.hpp"
#include "skyboxPG.hpp"
#include "skybox.hpp"
#include "glfw.hpp"
#include "transformBH.hpp"

#include <string>
#include <cstdio>
#include <vector>
#include <list>
#ifdef _WIN32
#include <direct.h>
#else
#include <unistd.h>
#endif

class Fps
{
public:
	Fps(int fps_val) {
		this->fps = fps_val;
		this->old_fps = fps_val;
		this->tick = 1.0 / this->fps;
		this->last_time = glfwGetTime();
		this->ellapsed_time = 0.0;
	}
	double				fps;
	double				old_fps;
	double				tick;
	double				ellapsed_time;
	double				last_time;

	bool		wait_for_next_frame() {
		this->ellapsed_time = glfwGetTime() - this->last_time;
		if (this->ellapsed_time >= this->tick)
		{
			this->last_time += this->ellapsed_time;
			this->ellapsed_time = 0.0;
			return (true);
		}
		else
			return (false);
	}
};

void	printFps() {
	static double last_time = 0;
	static double ellapsed_time = 0;
	double	current_time;
	double	fps;
	double	cent;
	current_time = glfwGetTime();
	ellapsed_time = current_time - last_time;
	fps = 1.0 / ellapsed_time;
	cent = fps - double(int(fps));
	if (cent >= 0.5)
		fps += 1.0;
	cout << (float)current_time << "\t" << int(fps) << "fps" << endl;
	last_time += ellapsed_time;
}

void	renderObj3d(list<Obj3d*>	obj3dList, Cam& cam) {
	// cout << "render all Obj3d" << endl;
	//assuming all Obj3d have the same program
	Obj3d*		obj = *(obj3dList.begin());
	Obj3dPG&	pg = obj->getProgram();
	glUseProgram(pg._program);//used once for all obj3d
	Math::Matrix4	proMatrix(cam.getProjectionMatrix());
	Math::Matrix4	viewMatrix = cam.getViewMatrix();
	proMatrix.mult(viewMatrix);// do it in shader ? NO cauz shader will do it for every vertix
	for (Obj3d* object : obj3dList)
		object->render(proMatrix);
	for (Obj3d* object : obj3dList) {
		object->local._matrixChanged = false;
		object->_worldMatrixChanged = false;
	}
}

void	renderSkybox(Skybox& skybox, Cam& cam) {
	// cout << "render Skybox" << endl;
	SkyboxPG&	pg = skybox.getProgram();
	glUseProgram(pg._program);//used once
	
	Math::Matrix4	proMatrix(cam.getProjectionMatrix());
	Math::Matrix4&	viewMatrix = cam.getViewMatrix();
	proMatrix.mult(viewMatrix);

	skybox.render(proMatrix);
}

void	check_paddings() {
	//	cout << sizeof(BITMAPINFOHEADER) << " = " << sizeof(BMPINFOHEADER) << endl;
#ifdef _WIN322
	cout << sizeof(BITMAPFILEHEADER) << " = " << sizeof(BMPFILEHEADER) << endl;
	cout << "bfType\t" << offsetof(BMPINFOHEADERBITMAPFILEHEADER, bfType) << endl;
	cout << "bfSize\t" << offsetof(BITMAPFILEHEADER, bfSize) << endl;
	cout << "bfReserved1\t" << offsetof(BITMAPFILEHEADER, bfReserved1) << endl;
	cout << "bfReserved2\t" << offsetof(BITMAPFILEHEADER, bfReserved2) << endl;
	cout << "bfOffBits\t" << offsetof(BITMAPFILEHEADER, bfOffBits) << endl;
#endif//_WIN32
	cout << "unsigned short\t" << sizeof(unsigned short) << endl;
	cout << "unsigned long \t" << sizeof(unsigned long) << endl;
	cout << "long          \t" << sizeof(long) << endl;
	cout << "long long     \t" << sizeof(long long) << endl;
	cout << "int           \t" << sizeof(int) << endl;
	if (sizeof(BMPFILEHEADER) != 14 || sizeof(BMPINFOHEADER) != 40) {
		cerr << "Padding in structure, exiting..." << endl << endl;
		cout << "BMPFILEHEADER\t" << sizeof(BMPFILEHEADER) << endl;
		cout << "bfType     \t" << offsetof(BMPFILEHEADER, bfType) << endl;
		cout << "bfSize     \t" << offsetof(BMPFILEHEADER, bfSize) << endl;
		cout << "bfReserved1\t" << offsetof(BMPFILEHEADER, bfReserved1) << endl;
		cout << "bfReserved2\t" << offsetof(BMPFILEHEADER, bfReserved2) << endl;
		cout << "bfOffBits\t" << offsetof(BMPFILEHEADER, bfOffBits) << endl;
		cout << "-----" << endl;
		cout << "BMPINFOHEADER\t" << sizeof(BMPINFOHEADER) << endl;
		cout << "biSize     \t" << offsetof(BMPINFOHEADER, biSize) << endl;
		cout << "biWidth    \t" << offsetof(BMPINFOHEADER, biWidth) << endl;
		cout << "biHeight\t" << offsetof(BMPINFOHEADER, biHeight) << endl;
		cout << "biPlanes\t" << offsetof(BMPINFOHEADER, biPlanes) << endl;
		cout << "biBitCount\t" << offsetof(BMPINFOHEADER, biBitCount) << endl;
		cout << "biCompression\t" << offsetof(BMPINFOHEADER, biCompression) << endl;
		cout << "biSizeImage\t" << offsetof(BMPINFOHEADER, biSizeImage) << endl;
		cout << "biXPelsPerMeter\t" << offsetof(BMPINFOHEADER, biXPelsPerMeter) << endl;
		cout << "biYPelsPerMeter\t" << offsetof(BMPINFOHEADER, biYPelsPerMeter) << endl;
		cout << "biClrUsed\t" << offsetof(BMPINFOHEADER, biClrUsed) << endl;
		cout << "biClrImportant\t" << offsetof(BMPINFOHEADER, biClrImportant) << endl;
		exit(ERROR_PADDING);
	}
}

class AnchorCameraBH : public Behavior
{
	/*
		La rotation fonctionne bien sur la cam (ca a l'air),
		mais le probleme vient de l'ordre de rotation sur l'anchor.
		? Ne pas utiliser ce simple system de rotation
		? rotater par rapport au system local de l'avion
		? se baser sur une matrice cam-point-at
			https://mikro.naprvyraz.sk/docs/Coding/Atari/Maggie/3DCAM.TXT
	*/
public:
	AnchorCameraBH() : Behavior() {
		this->copyRotation = true;
	}
	void	behaveOnTarget(BehaviorManaged* target) {
		if (this->_anchor) {
			Object*	speAnchor = dynamic_cast<Object*>(this->_anchor);//specialisation part
			// turn this in Obj3d to get the BP, to get the size of the ovject,
			// to position the camera relatively to the obj's size.

			Cam*	speTarget = dynamic_cast<Cam*>(target);//specialisation part

			Math::Vector3	forward(0, -15, -35);
			if (this->copyRotation) {
				Math::Rotation	rot = speAnchor->local.getRot();
				forward.rotate(rot, 1);
				rot.mult(-1);
				speTarget->local.setRot(rot);
			}
			forward.mult(-1);// invert the forward to position the cam on the back, a bit up
			Math::Vector3	pos = speAnchor->local.getPos();
			pos.translate(forward);
			speTarget->local.setPos(pos);
		}
	}
	bool	isCompatible(BehaviorManaged* target) const {
		//dynamic_cast check for Cam
		(void)target;
		return (true);
	}

	void			setAnchor(Object* anchor) {
		this->_anchor = anchor;
	}

	bool			copyRotation;
private:
	Object*			_anchor;
	Math::Vector3	_offset;

};

void	scene1() {
	Glfw	glfw(1600, 900);
	glfw.setTitle("This title is long, long enough to test how glfw manages oversized titles. At this point I dont really know what to write, so let's just bullshiting it ....................................................... is that enough? Well, it depends of the size of the current window. I dont really know how many characters i have to write for a width of 1920. Is it possible to higher the police ? It could save some characters. Ok, im bored, lets check if this title is long enough!");

	//Program for Obj3d (can render Obj3d) with vertex & fragmetns shaders
	Obj3dPG			obj3d_prog(OBJ3D_VS_FILE, OBJ3D_FS_FILE);

#ifndef BLUEPRINTS
	//Create Obj3dBP from .obj files
	Obj3dBP			the42BP("obj3d/42.obj", true);
	Obj3dBP			cubeBP("obj3d/cube.obj", true);
	Obj3dBP			teapotBP("obj3d/teapot2.obj", true);
	// Obj3dBP			rocketBP("obj3d/Rocket_Phoenix/AIM-54_Phoenix_OBJ/Aim-54_Phoenix.obj", true);
	Obj3dBP			rocketBP("obj3d/ARSENAL_VG33/Arsenal_VG33.obj", true);
	// Obj3dBP			lamboBP("obj3d/lambo/Lamborginhi_Aventador_OBJ/Lamborghini_Aventador.obj", true);
	Obj3dBP			lamboBP("obj3d/lambo/Lamborginhi_Aventador_OBJ/Lamborghini_Aventador_no_collider.obj", true);
	cout << "======" << endl;
#endif
#ifndef TEXTURES
		Texture*	texture1 = new Texture("images/lena.bmp");
		Texture*	texture2 = new Texture("images/skybox2.bmp");
		Texture*	texture3 = new Texture("images/skyboxfuck.bmp");
	//	Texture*	texture4 = new Texture("images/skybox4096.bmp");
		Texture*	texture5 = new Texture("images/skytest.bmp");
		// Texture*	texture6 = new Texture("obj3d/Rocket_Phoenix/AIM-54_Phoenix_OBJ/Phoenix.bmp");
		// Texture*	texture6 = new Texture("obj3d/ARSENAL_VG33/Arsenal_VG33.bmp");
	//	Texture*	texture7 = new Texture("obj3d/lambo/Lamborginhi_Aventador_OBJ/Lamborginhi_Aventador_diffuse.bmp");
		Texture		texture8 = *texture1;
#endif
#ifndef OBJ3D
	list<Obj3d*>	obj3dList;

	float s = 1.0f;//scale
	//Create Obj3d with the blueprint & by copy
	Obj3d			the42_1(cubeBP, obj3d_prog);//the42BP !
		the42_1.local.setPos(0, 0, 0);
		the42_1.setTexture(texture1);
		the42_1.displayTexture = true;
		the42_1.setPolygonMode(GL_FILL);
		the42_1.local.setScale(1, 1, 1);
		the42_1.local.centered = true;

	Obj3d			the42_2(the42_1);
		//the42_2.local.setPos(0, 3, -5);
		the42_2.local.setPos(-4, -2, -2);
		the42_2.setTexture(texture2);
		the42_2.displayTexture = false;
		the42_2.local.centered = true;
		// the42_2.setScale(3.0f, 0.75f, 0.3f);
		the42_2.setPolygonMode(GL_LINE);

	Obj3d			teapot1(teapotBP, obj3d_prog);
		teapot1.local.setPos(0, 0, 2);
		teapot1.local.getMatrix().setOrder(ROW_MAJOR);
		// teapot1.setRot(0, 90, 0);
		teapot1.setTexture(texture1);
		teapot1.displayTexture = false;
		teapot1.setPolygonMode(GL_LINE);
		// teapot1.setScale(1.5, 2, 0.75);

	Obj3d			cube1(cubeBP, obj3d_prog);
		cube1.local.setPos(0, -2, 3);
		cube1.setTexture(texture1);
		cube1.displayTexture = false;

	Object			empty1;
		empty1.local.setScale(1,1,1);

	Obj3d			rocket1(rocketBP, obj3d_prog);
		// rocket1.local.setPos(-10, -20, -2000);
		rocket1.local.setPos(0, -300, 0);
		rocket1.local.rotate(0, 180, 0);
		rocket1.setTexture(texture5);
		rocket1.displayTexture = true;
		rocket1.local.centered = true;
		// rocket1.setPolygonMode(GL_LINE);
		s = 10.0f;
		rocket1.local.setScale(s,s,s);
		rocket1.setParent(&empty1);


	// Properties::defaultSize = 13.0f;
	Obj3d			lambo1(lamboBP, obj3d_prog);
		lambo1.local.setPos(-20, 0, 0);
		lambo1.local.setPos(0, -5, 7);
		lambo1.setTexture(texture1);
		lambo1.displayTexture = true;
		lambo1.local.centered = true;
		lambo1.setPolygonMode(GL_LINE);
		s = 0.025f;
		// lambo1.setScale(s, s, s);

	Obj3d			lambo2(lamboBP, obj3d_prog);
		// lambo2.local.setPos(0, -1.9f, 0);
		lambo2.local.setPos(0, -6.0f, 0);
		lambo2.local.setRot(0, 180.0f, 0);
		lambo2.setTexture(texture1);
		lambo2.displayTexture = true;
		lambo2.local.centered = true;
		// lambo2.setPolygonMode(GL_LINE);
		s = 0.4f;
		// lambo2.local.setScale(s, s, s);
		// lambo2.setParent(&the42_1);
		lambo2.setParent(&rocket1);

	Obj3d			lambo3(lamboBP, obj3d_prog);
		lambo3.local.setPos(0, -4, 0);
		lambo3.local.setRot(0, 0.0f, 180);
		lambo3.setTexture(&texture8);
		lambo3.displayTexture = true;
		lambo3.local.centered = true;
		// lambo3.setPolygonMode(GL_LINE);
		s = 30.0f;
		// lambo3.local.setScale(s, s, s);
		// lambo3.setParent(&the42_1);
		lambo3.setParent(&lambo2);

		obj3dList.push_back(&the42_1);
		// obj3dList.push_back(&the42_2);
		obj3dList.push_back(&teapot1);
		// obj3dList.push_back(&cube1);
		obj3dList.push_back(&rocket1);
		obj3dList.push_back(&lambo1);
		obj3dList.push_back(&lambo2);
		obj3dList.push_back(&lambo3);

		if (false) {//spiral
			// Obj3d*	backObj = &lambo3;
			for (int i = 0; i < 20; i++) {
				Obj3d* lamboPlus = new Obj3d(lamboBP, obj3d_prog);
				lamboPlus->setParent(&lambo3);
				lamboPlus->displayTexture = (i % 2) ? true : false;
				lamboPlus->local.centered = true;
				lamboPlus->setTexture(&texture8);
				float maxScale = 3;
				float scale = (float)((i % (int)maxScale) - (maxScale / 2));
				lamboPlus->local.enlarge(scale, scale, scale);
				float	val = cosf(Math::toRadian(i * 10)) * 10;
				float	coef = 1.0f;
				lamboPlus->local.setPos(lambo3.local.getPos());
				lamboPlus->local.translate(float(i) / coef, val / coef, val / coef);
				lamboPlus->local.rotate(Math::Rotation(i * 5, i * 5, i * 5));

				obj3dList.push_back(lamboPlus);
				// backObj = lamboPlus;
			}
		}
#endif


	// Properties::defaultSize = PP_DEFAULT_SIZE;

	cout << &rocket1 << endl;
	// test_behaviors(rocket1);

	cout << "Object # : " << Object::getInstanceAmount() << endl;
	cout << "Obj3d # : " << Obj3d::getInstanceAmount() << endl;
	cout << endl;
	cout << "GL_MAX_CUBE_MAP_TEXTURE_SIZE " << GL_MAX_CUBE_MAP_TEXTURE_SIZE << endl;
	cout << "GL_MAX_TEXTURE_SIZE " << GL_MAX_TEXTURE_SIZE << endl;

	SkyboxPG	sky_pg(CUBEMAP_VS_FILE, CUBEMAP_FS_FILE);
	Skybox		skybox(*texture3, sky_pg);

#ifndef CAM
	Cam		cam(glfw);
	cam.local.centered = false;
	cam.local.setPos(0, 0, 10);
	cam.printProperties();
	cam.lockedMovement = false;
	cam.lockedOrientation = false;

	glfw.setMouseAngle(-1);
	std::cout << "MouseAngle: " <<  glfw.getMouseAngle() << std::endl;
	//exit(0);

	if (false) {//cam anchor to rocket1, bugged with Z rot
		cam.local.setPos(0, 1.5f, 3.5f);
		cam.setParent(&rocket1);
		cam.lockedMovement = true;
		cam.lockedOrientation = true;
	}
#endif

	Fps	fps144(144);
	Fps	fps60(60);
	Fps	fps30(30);
	Fps* defaultFps = &fps144;

	//followObjectArgs	st = { defaultFps, &cam };


#ifndef BEHAVIORS
	cout << "behavior:" << endl;
	TransformBH		b1;
		b1.transform.rot.setUnit(ROT_DEG);
		b1.transform.rot.z = 10 * defaultFps->tick;
		b1.modeRot = ADDITIVE;
		float ss = 1.0f + 0.1f * defaultFps->tick;
		//b1.transform.scale = Math::Vector3(ss, ss, ss);
	//	b1.modeScale = MULTIPLICATIVE;
		cout << "___ adding rocket1: " << &rocket1 << endl;
		b1.addTarget(&rocket1);
		b1.addTarget(&lambo1);
		b1.removeTarget(&lambo1);
		b1.setTargetStatus(&rocket1, false);
		std::cout << "rocket1 status: " << b1.getTargetStatus(&rocket1) << std::endl;
		b1.setTargetStatus(&rocket1, true);
		std::cout << "rocket1 status: " << b1.getTargetStatus(&rocket1) << std::endl;

	TransformBH		b2;// = b1;//bug
		b2.transform.scale = Math::Vector3(0,0,0);
		b2.modeScale = ADDITIVE;
		b2.transform.rot.z = 0.0f;
		b2.transform.rot.x = -45.0f * defaultFps->tick;
		// b2.removeTarget(&rocket1);
		b2.addTarget(&empty1);
		//bug if i do:
		b2.addTarget(&empty1);
		b2.removeTarget(&rocket1);
	
		cout << "b1: " << b1.getTargetList().size() << endl;
		cout << "b2: " << b2.getTargetList().size() << endl;
	// exit(0);

	if (false) {// check behavior target, add remove

		cout << "behaviorsActive: " << (empty1.behaviorsActive ? "true" : "false") << endl;
		cout << "------------" << endl;
		cout << "lambo1:\t" << lambo1.behaviorList.size() << endl;
		cout << "b2:    \t" << b2.getTargetList().size() << endl;
		cout << "b2.addTarget(&lambo1);" << endl;
		b2.addTarget(&lambo1);
		cout << "lambo1:\t" << lambo1.behaviorList.size() << endl;
		cout << "b2:    \t" << b2.getTargetList().size() << endl;
		cout << "b2.removeTarget(&lambo1);" << endl;
		b2.removeTarget(&lambo1);
		cout << "lambo1:\t" << lambo1.behaviorList.size() << endl;
		cout << "b2:    \t" << b2.getTargetList().size() << endl;

		cout << "------------" << endl;
		cout << "lambo1:\t" << lambo1.behaviorList.size() << endl;
		cout << "b2:    \t" << b2.getTargetList().size() << endl;
		cout << "lambo1.addBehavior(&b2);" << endl;
		lambo1.addBehavior(&b2);
		cout << "lambo1:\t" << lambo1.behaviorList.size() << endl;
		cout << "b2:    \t" << b2.getTargetList().size() << endl;
		cout << "lambo1.removeBehavior(&b2);" << endl;
		lambo1.removeBehavior(&b2);
		cout << "lambo1:\t" << lambo1.behaviorList.size() << endl;
		cout << "b2:    \t" << b2.getTargetList().size() << endl;
	}
#endif
// exit(0);

#ifndef RENDER
	cout << "Begin while loop" << endl;
	// cam.local.setScale(s,s,s);//bad, undefined behavior
	while (!glfwWindowShouldClose(glfw._window)) {
		if (defaultFps->wait_for_next_frame()) {
			//////////////////////////////////////////
			if (true) {
				if (false) {
					Math::Matrix4	matRocket = rocket1.getWorldMatrix();
					matRocket.printData();
					cout << "---------------" << endl;
				}
				if (false) {
					the42_1.getWorldMatrix().printData();
				}
				if (false) {
					cout << "---rocket1" << endl;
					rocket1.local.getScale().printData();
					// rocket1.getWorldMatrix().printData();
					cout << "---lambo2" << endl;
					lambo2.local.getScale().printData();
					// lambo2.getWorldMatrix().printData();
					cout << "---lambo3" << endl;
					lambo3.local.getScale().printData();
					// lambo3.getWorldMatrix().printData();
					cout << "---------------" << endl;
					lamboBP.getDimensions().printData();
					cout << "---------------" << endl;
				}
			}
			////////////////////////////////////////// motion/behaviors
			//this should be used in another func, life a special func managing all events/behavior at every frames
			if (true) {
				b1.run();
				b2.run();
				// Math::Matrix4	matEmpty = empty1.getWorldMatrix();
				// matEmpty.printData();
			}
			////////////////////////////////////////// motion end

			glfwPollEvents();
			glfw.updateMouse();//to do before cam's events
			cam.events(glfw, float(defaultFps->tick));
			// printFps();
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			renderObj3d(obj3dList, cam);
			renderSkybox(skybox, cam);
			glfwSwapBuffers(glfw._window);

			if (GLFW_PRESS == glfwGetKey(glfw._window, GLFW_KEY_ESCAPE))
				glfwSetWindowShouldClose(glfw._window, GLFW_TRUE);
		}
	}
	cout << "End while loop" << endl;
#endif

	cout << "deleting textures..." << endl;
	delete texture1;
	delete texture2;
	delete texture3;
//	delete texture4;
	delete texture5;
//	delete texture6;
	//delete texture7;
}

void scene2() {
	Glfw	glfw(1600, 900);
	glfw.setTitle("Tests camera anchor");

	//Program for Obj3d (can render Obj3d) with vertex & fragmetns shaders
	Obj3dPG			obj3d_prog(OBJ3D_VS_FILE, OBJ3D_FS_FILE);
	SkyboxPG		sky_pg(CUBEMAP_VS_FILE, CUBEMAP_FS_FILE);

	Obj3dBP			rocketBP("obj3d/ARSENAL_VG33/Arsenal_VG33.obj", true);

	Texture* texture3 = new Texture("images/skyboxfuck.bmp");
	Texture* texture5 = new Texture("images/skytest.bmp");

	Skybox		skybox(*texture3, sky_pg);


	list<Obj3d*>	obj3dList;

	Obj3d			rocket(rocketBP, obj3d_prog);
	rocket.local.setPos(0, 0, 0);
	rocket.setTexture(texture5);
	rocket.displayTexture = true;
	rocket.local.centered = true;
	float s = 10.0f;
	rocket.local.setScale(s, s, s);

	Obj3d			rocket2(rocketBP, obj3d_prog);
	rocket2.local.setPos(5, 0, 0);
	rocket2.setTexture(texture5);
	rocket2.displayTexture = true;
	rocket2.local.centered = true;
	s = 5000.0f;
	rocket2.local.setScale(s, s, s);

	obj3dList.push_back(&rocket);
	obj3dList.push_back(&rocket2);


	Cam		cam(glfw);
	cam.local.centered = false;
	cam.local.setPos(0, 15, 35);
	cam.printProperties();
	cam.lockedMovement = false;
	cam.lockedOrientation = false;

	glfw.setMouseAngle(-1);
	std::cout << "MouseAngle: " << glfw.getMouseAngle() << std::endl;
	//exit(0);

	//cam.local.setPos(0, 1.5f, 3.5f);
	AnchorCameraBH	b1;
	b1.setAnchor(&rocket);
	b1.addTarget(&cam);
	b1.copyRotation = true;
	cam.lockedMovement = true;
	cam.lockedOrientation = true;


	Fps	fps144(144);
	Fps	fps60(60);
	Fps* defaultFps = &fps144;

	float	mvt = 30.0f * defaultFps->tick;

	std::cout << "Begin while loop" << endl;
	int c = 0;
	while (!glfwWindowShouldClose(glfw._window)) {
		if (defaultFps->wait_for_next_frame()) {

			if (1) {
				b1.run();
			}

			//print data
			if (false) {
				c++;
				if (c == 60) {
					c = 0;
					system("cls");
					if (true) {
						std::cout << "--- ROCKET" << endl;
						//rocket.local.getScale().printData();
						rocket.local.getRot().printData();
						rocket.getWorldMatrix().printData();
					}
					if (true) {
						std::cout << "--- CAM" << endl;
						cam.local.getRot().printData();
						std::cout << "- local" << endl;
						cam.getLocalProperties().getMatrix().printData();
						std::cout << "- world" << endl;
						cam.getWorldMatrix().printData();
					}
				}
			}

			glfwPollEvents();
			glfw.updateMouse();//to do before cam's events
			cam.events(glfw, float(defaultFps->tick));
			if (true) {
				if (GLFW_PRESS == glfwGetKey(glfw._window, GLFW_KEY_LEFT))
					rocket.local.rotate(0,		mvt,	0);
				if (GLFW_PRESS == glfwGetKey(glfw._window, GLFW_KEY_RIGHT))
					rocket.local.rotate(0,		-mvt,	0);
				if (GLFW_PRESS == glfwGetKey(glfw._window, GLFW_KEY_DOWN))
					rocket.local.rotate(mvt,	0,		0);
				if (GLFW_PRESS == glfwGetKey(glfw._window, GLFW_KEY_UP))
					rocket.local.rotate(-mvt,	0,		0);
				if (GLFW_PRESS == glfwGetKey(glfw._window, GLFW_KEY_KP_1))
					rocket.local.rotate(0,		0,		mvt);
				if (GLFW_PRESS == glfwGetKey(glfw._window, GLFW_KEY_KP_2))
					rocket.local.rotate(0,		0,		-mvt);
				if (GLFW_PRESS == glfwGetKey(glfw._window, GLFW_KEY_KP_0))
					rocket.local.setRot(0,		0,		0);

				Math::Vector3	pos = rocket.local.getPos();
				if (GLFW_PRESS == glfwGetKey(glfw._window, GLFW_KEY_KP_8))
					pos.add(0, 0, -mvt);
				if (GLFW_PRESS == glfwGetKey(glfw._window, GLFW_KEY_KP_5))
					pos.add(0, 0, mvt);
				if (GLFW_PRESS == glfwGetKey(glfw._window, GLFW_KEY_KP_4))
					pos.add(-mvt, 0, 0);
				if (GLFW_PRESS == glfwGetKey(glfw._window, GLFW_KEY_KP_6))
					pos.add(mvt, 0, 0);
				if (GLFW_PRESS == glfwGetKey(glfw._window, GLFW_KEY_KP_7))
					pos.add(0, -mvt, 0);
				if (GLFW_PRESS == glfwGetKey(glfw._window, GLFW_KEY_KP_9))
					pos.add(0, mvt, 0);
				rocket.local.setPos(pos);


//#define 	GLFW_KEY_RIGHT   262
//#define 	GLFW_KEY_LEFT   263
//#define 	GLFW_KEY_DOWN   264
//#define 	GLFW_KEY_UP   265
			}

			// printFps();
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			renderObj3d(obj3dList, cam);
			renderSkybox(skybox, cam);
			glfwSwapBuffers(glfw._window);

			if (GLFW_PRESS == glfwGetKey(glfw._window, GLFW_KEY_ESCAPE))
				glfwSetWindowShouldClose(glfw._window, GLFW_TRUE);
		}
	}
	std::cout << "End while loop" << endl;

	std::cout << "deleting textures..." << endl;
	delete texture3;
	delete texture5;
}


void sceneHumanGL() {
	Glfw		glfw(1600, 900); // screen size
	glDisable(GL_CULL_FACE);
	Obj3dPG		obj3d_prog(OBJ3D_VS_FILE, OBJ3D_FS_FILE);
	SkyboxPG	sky_pg(CUBEMAP_VS_FILE, CUBEMAP_FS_FILE);

	Obj3dBP::defaultSize = 1.0f;
	Obj3dBP		cubebp("obj3d/cube.obj", true);

	Texture *	lena = new Texture("images/lena.bmp");

#ifndef MEMBERS
	float		epaisseur_bras = 1.0f;
	float		epaisseur_tronc = 2.0f;
	bool		centerCubes = false;

	Object			containerTronc;
	Obj3d			tronc(cubebp, obj3d_prog);
	tronc.setTexture(lena);
	tronc.displayTexture = true;
	tronc.local.setScale(epaisseur_tronc, epaisseur_tronc * 3, epaisseur_tronc);
	tronc.setColor(0xff, 0, 0);
	tronc.local.centered = centerCubes;

	Obj3d			avant_bras_gauche(cubebp, obj3d_prog);
	avant_bras_gauche.displayTexture = false;
	avant_bras_gauche.local.setScale(epaisseur_bras, epaisseur_tronc * 2, epaisseur_bras);
	avant_bras_gauche.setColor(0, 0xff, 0);
	avant_bras_gauche.local.centered = centerCubes;

	Obj3d			avant_bras_droit(cubebp, obj3d_prog);
	avant_bras_droit.displayTexture = false;
	avant_bras_droit.local.setScale(-epaisseur_bras, epaisseur_tronc * 2, epaisseur_bras);
	avant_bras_droit.setColor(0, 0, 0xff);
	avant_bras_droit.local.centered = centerCubes;

	Obj3d			apres_bras_gauche(cubebp, obj3d_prog);
	apres_bras_gauche.displayTexture = false;
	apres_bras_gauche.setColor(0xf0, 0xf0, 0);
	apres_bras_gauche.local.centered = centerCubes;

	Obj3d			apres_bras_droit(cubebp, obj3d_prog);
	apres_bras_droit.displayTexture = false;
	apres_bras_droit.setColor(0, 0xf0, 0xf0);
	apres_bras_droit.local.centered = centerCubes;

	//hierarchy
	tronc.setParent(&containerTronc);
	avant_bras_gauche.setParent(&containerTronc);
		apres_bras_gauche.setParent(&avant_bras_gauche);
	avant_bras_droit.setParent(&containerTronc);
		apres_bras_droit.setParent(&avant_bras_droit);

	//relative position
	avant_bras_gauche.local.translate(-epaisseur_bras, 0, 0);
	avant_bras_droit.local.translate(epaisseur_tronc, 0, 0);
	apres_bras_gauche.local.translate(VEC3_DOWN);
	apres_bras_droit.local.translate(VEC3_DOWN);


	list<Obj3d*>	obj3dList;
	// obj3dList.push_back(&containerTronc);
	obj3dList.push_back(&tronc);
	obj3dList.push_back(&avant_bras_gauche);
	obj3dList.push_back(&avant_bras_droit);
	obj3dList.push_back(&apres_bras_droit);
	obj3dList.push_back(&apres_bras_gauche);
#endif // MEMBERS

	Texture*	texture2 = new Texture("images/skybox4.bmp");//skybox3.bmp bug?
	Skybox		skybox(*texture2, sky_pg);
	
	Cam		cam(glfw);
	cam.local.centered = false;
	cam.local.setPos(0, 0, 10);
	cam.setFov(90);
	cam.lockedMovement = false;
	cam.lockedOrientation = false;
	cam.speed /= 4;

	Fps	fps144(144);
	Fps	fps60(60);
	Fps* defaultFps = &fps60;

	TransformBH		b_rotZ;
	b_rotZ.transform.rot.setUnit(ROT_DEG);
	b_rotZ.transform.rot.z = 100 * defaultFps->tick;
	b_rotZ.modeRot = ADDITIVE;
	b_rotZ.addTarget(&avant_bras_droit);
	b_rotZ.addTarget(&containerTronc);

	TransformBH		b_rotY;
	b_rotY.transform.rot.setUnit(ROT_DEG);
	b_rotY.transform.rot.y = 720 * defaultFps->tick;
	b_rotY.modeRot = ADDITIVE;
	b_rotY.addTarget(&avant_bras_gauche);


	while (!glfwWindowShouldClose(glfw._window)) {
		if (defaultFps->wait_for_next_frame()) {
			b_rotZ.run();
			b_rotY.run();

			glfwPollEvents();
			glfw.updateMouse(); // to do before cam's events
			cam.events(glfw, float(defaultFps->tick));
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			renderObj3d(obj3dList, cam);
			renderSkybox(skybox, cam);
			glfwSwapBuffers(glfw._window);
			if (GLFW_PRESS == glfwGetKey(glfw._window, GLFW_KEY_ESCAPE))
				glfwSetWindowShouldClose(glfw._window, GLFW_TRUE);
		}
	}
}

int		main(void) {
	check_paddings();
	// test_behaviors();
//	test_mult_mat4(); exit(0);
	std::cout << "____START____" << endl;
//	test_obj_loader();

	//scene1();
	// scene2();
	sceneHumanGL();
	// while(1);

	return (EXIT_SUCCESS);
}
