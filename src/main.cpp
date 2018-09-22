/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rhoffsch <rhoffsch@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/18 22:45:30 by rhoffsch          #+#    #+#             */
/*   Updated: 2018/09/20 09:38:45 by rhoffsch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "simplegl.h"

#include "program.hpp"
#include "obj3dPG.hpp"
#include "obj3dBP.hpp"
#include "obj3d.hpp"
#include "misc.hpp"
#include "cam.hpp"
#include "texture.hpp"
#include "skyboxPG.hpp"
#include "skybox.hpp"
#include "glfw.hpp"

#include <string>
#include <cstdio>
#include <vector>
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

void	renderObj3d(vector<Obj3d*>	obj3dList, Cam& cam) {
//	cout << "render all Obj3d" << endl;
	//assuming all Obj3d have the same program
	Obj3d*		obj = *(obj3dList.begin());
	Obj3dPG&	pg = obj->getProgram();
	glUseProgram(pg._program);
	Math::Matrix4	proMatrix(cam.getProjectionMatrix());
	Math::Matrix4	viewMatrix = cam.getViewMatrix();
	proMatrix.mult(viewMatrix);
	for (Obj3d* object : obj3dList)
		object->render(proMatrix);
}

void	renderSkybox(Skybox& skybox, Cam& cam) {
//	cout << "render Skybox" << endl;
	SkyboxPG&	pg = skybox.getProgram();
	glUseProgram(pg._program);
	
	Math::Matrix4	proMatrix(cam.getProjectionMatrix());
	Math::Matrix4&	viewMatrix = cam.getViewMatrix();
	proMatrix.mult(viewMatrix);

	skybox.render(proMatrix);
}

void	check_paddings() {
//	cout << sizeof(BITMAPINFOHEADER) << " = " << sizeof(BMPINFOHEADER) << endl;
#ifdef _WIN32
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

void	growAndShrink(Obj3d & ref, void* ptr) {
	static float	growCoef = 1;

	Fps * fps_ptr = (Fps*)ptr;
	Math::Vector3	s = ref.getScale();
	float	addScale = (s.x / 2.0f) * (float)fps_ptr->tick;
	addScale *= growCoef;
	s.add(Math::Vector3(addScale, addScale, addScale));
	ref.setScale(s);
	
	//alternate grow/shrink each second
	int	t = int(fps_ptr->last_time) % 2;
	growCoef = (t == 0) ? -1 : 1;
}

void	rotAndGoZaxis(Obj3d & ref, void* ptr) {
	static float	anglePerSec = 75;
	static float	distPerSec = 250;
	
	Fps * fps_ptr = (Fps*)ptr;
	Math::Rotation	rot = ref.getRot();
	Math::Vector3	pos = ref.getPos();
	rot.setAsDegree();
	rot.z += anglePerSec * (float)fps_ptr->tick;
	ref.setRot(rot);
	pos.add(Math::Vector3(0, 0, distPerSec * (float)fps_ptr->tick));
	ref.setPos(pos);
}

struct	followCamArgs
{
	Fps *	fps;
	Cam *	cam;
};
void	followCam(Obj3d & ref, void *ptr) {
	followCamArgs * st = (followCamArgs*)ptr;
	Math::Vector3	diff = st->cam->getPos();
	Math::Vector3	objPos = ref.getPos();
	diff.sub(objPos);
	float	magnitude = diff.magnitude();
	if (magnitude > 0.1f) {
		diff.div(magnitude);
		diff.mult(2 * st->fps->tick);
		objPos.add(diff);
		ref.setPos(objPos);
	}
}

void	scene1() {
	Glfw	glfw(1600, 900);
	glfw.setTitle("This title is long, long enough to test how glfw manages oversized titles. At this point I dont really know what to write, so let's just bullshiting it ....................................................... is that enough? Well, it depends of the size of the current window. I dont really know how many characters i have to write for a width of 1920. Is it possible to higher the police ? It could save some characters. Ok, im bored, lets check if this title is long enough!");

	//Program for Obj3d (can render Obj3d) with vertex & fragmetns shaders
	Obj3dPG			obj3d_prog(OBJ3D_VS_FILE, OBJ3D_FS_FILE);

	//Create Obj3dBP from .obj files
	Obj3dBP			the42BP("obj3d/42.obj");
	Obj3dBP			cubeBP("obj3d/cube.obj");
	Obj3dBP			teapotBP("obj3d/teapot2.obj");
	// Obj3dBP			rocketBP("obj3d/Rocket_Phoenix/AIM-54_Phoenix_OBJ/Aim-54_Phoenix.obj");
	Obj3dBP			rocketBP("obj3d/ARSENAL_VG33/Arsenal_VG33.obj");
	// Obj3dBP			lamboBP("obj3d/lambo/Lamborginhi_Aventador_OBJ/Lamborghini_Aventador.obj");
	Obj3dBP			lamboBP("obj3d/lambo/Lamborginhi_Aventador_OBJ/Lamborghini_Aventador_no_collider.obj");
	cout << "======" << endl;

	Texture*	texture1 = new Texture("images/lena.bmp");
	Texture*	texture2 = new Texture("images/skybox2.bmp");
	Texture*	texture3 = new Texture("images/skyboxfuck.bmp");
	Texture*	texture4 = new Texture("images/skybox4096.bmp");
	Texture*	texture5 = new Texture("images/skytest.bmp");
	// Texture*	texture6 = new Texture("obj3d/Rocket_Phoenix/AIM-54_Phoenix_OBJ/Phoenix.bmp");
	Texture*	texture6 = new Texture("obj3d/ARSENAL_VG33/Arsenal_VG33.bmp");
	Texture*	texture7 = new Texture("obj3d/lambo/Lamborginhi_Aventador_OBJ/Lamborginhi_Aventador_diffuse.bmp");

	float s = 1.0f;//scale
	//Create Obj3d with the blueprint & by copy
	Obj3d			the42_1(cubeBP, obj3d_prog);//the42BP !
	the42_1.setPos(-4, -2, -2);
	the42_1.setTexture(texture1);
	the42_1.displayTexture = true;
	the42_1.setPolygonMode(GL_FILL);
	// the42_1.centered = true;

	Obj3d	the42_2(the42_1);
	//the42_2.setPos(0, 3, -5);
	the42_2.setPos(-4, -2, -2);
	the42_2.setTexture(texture2);
	the42_2.displayTexture = false;
	the42_2.centered = true;
	// the42_2.setScale(3.0f, 0.75f, 0.3f);
	the42_2.setPolygonMode(GL_LINE);

	Obj3d			teapot1(teapotBP, obj3d_prog);
	teapot1.setPos(0, 0, 2);
	teapot1.getModelMatrix().setOrder(ROW_MAJOR);
//	teapot1.setRot(0, 90, 0);
	teapot1.setTexture(texture1);
	teapot1.displayTexture = false;
	teapot1.setPolygonMode(GL_LINE);
	teapot1._motionBehaviorFunc = &followCam;
	teapot1._motionBehavior = true;
	// teapot1.setScale(1.5, 2, 0.75);

	Obj3d			cube1(cubeBP, obj3d_prog);
	cube1.setPos(0, -2, 3);
	cube1.setTexture(texture1);
	cube1.displayTexture = false;

	Obj3d			rocket1(rocketBP, obj3d_prog);
	rocket1.setPos(-10, -20, -2000);
	rocket1.setTexture(texture6);
	rocket1.displayTexture = true;
	rocket1.centered = true;
	// rocket1.setRescaled(false);
	// rocket1.setPolygonMode(GL_LINE);
	rocket1._motionBehaviorFunc = &rotAndGoZaxis;
	rocket1._motionBehavior = true;
	s = 100.0f;
	rocket1.setScale(s,s,-s);

	Obj3d::defaultSize = 13.0f;
	Obj3d			lambo1(lamboBP, obj3d_prog);
	lambo1.setPos(-20, 0, 0);
	lambo1.setTexture(texture7);
	lambo1.displayTexture = true;
	// lambo1.centered = true;
	// lambo1.setPolygonMode(GL_LINE);
	lambo1._motionBehaviorFunc = &growAndShrink;
	lambo1._motionBehavior = true;
	s = 0.025f;
	// lambo1.setScale(s, s, s);
	Obj3d::defaultSize = OBJ3D_DEFAULT_SIZE;

	cout << "Obj3d # : " << Obj3d::getInstanceAmount() << endl;
	cout << endl;

	cout << "GL_MAX_CUBE_MAP_TEXTURE_SIZE " << GL_MAX_CUBE_MAP_TEXTURE_SIZE << endl;
	cout << "GL_MAX_TEXTURE_SIZE " << GL_MAX_TEXTURE_SIZE << endl;

	SkyboxPG	sky_pg(CUBEMAP_VS_FILE, CUBEMAP_FS_FILE);
	Skybox		skybox(*texture4, sky_pg);
	
	vector<Obj3d*>	obj3dList;
	obj3dList.push_back(&the42_1);
	obj3dList.push_back(&the42_2);
	obj3dList.push_back(&teapot1);
	// obj3dList.push_back(&cube1);
	obj3dList.push_back(&rocket1);
	obj3dList.push_back(&lambo1);

	
	Cam		cam(glfw);
	cam.setPos(0, 0, 10);
	cam.printProperties();

	cout << "Begin while loop" << endl;
	Fps	fps144(144);
	Fps	fps60(60);
	Fps	fps30(30);
	Fps* defaultFps = &fps60;

	followCamArgs	st = { defaultFps, &cam };

	while (!glfwWindowShouldClose(glfw._window)) {
		if (defaultFps->wait_for_next_frame()) {
			// printFps();
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			renderSkybox(skybox, cam);
			renderObj3d(obj3dList, cam);
			glfwSwapBuffers(glfw._window);
			glfwPollEvents();
			//events(glfw, &gle, &progs[OBJ3D]);
			glfw.updateMouse();//to do before cam's events
			cam.events(glfw, float(defaultFps->tick));
			
			//////////////////////////////////////////manual motion
			float	v1 = 160;
			float	v2 = 360;// degree/sec
			Math::Rotation rot = the42_1.getRot();
			rot.setAsDegree();
			// rot.x += v1 * (float)defaultFps->tick;
			rot.y += v2 * (float)defaultFps->tick;
			the42_1.setRot(rot);

			//this should be used in another func, life a special func managing all events/behavior at every frames
			rocket1.runMothionBehavior((void*)defaultFps);
			lambo1.runMothionBehavior((void*)defaultFps);
			teapot1.runMothionBehavior((void*)&st);

			// Fps * fps_ptr = &fps30;
			// if (fps_ptr->wait_for_next_frame()) {
				v2 = 50;
				Obj3d*	ptr = &lambo1;
				rot = ptr->getRot();
				rot.setAsDegree();
			//	rot.x += v1 * (float)fps_ptr->tick;
				rot.y += v2 * (float)defaultFps->tick;
				ptr->setRot(rot);
			// }
			/*
			*/
			//////////////////////////////////////////manual motion end
		
			if (GLFW_PRESS == glfwGetKey(glfw._window, GLFW_KEY_ESCAPE))
				glfwSetWindowShouldClose(glfw._window, GLFW_TRUE);
		}
	}

	delete texture1;
	delete texture2;
	delete texture3;
	delete texture4;
	delete texture5;
	delete texture6;
	delete texture7;
}

int		main(void) {
	check_paddings();
//	test_mult_mat4(); exit(0);
	cout << "____START____" << endl;
//	test_obj_loader();

	scene1();
	// while(1);

	return (EXIT_SUCCESS);
}
