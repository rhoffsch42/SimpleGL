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

#include <iostream>
#include <random>
#include <ctime>

int		main(void) {
	check_paddings();
//	test_mult_mat4(); exit(0);
	cout << "____START____" << endl;
//	test_obj_loader();
	Glfw	glfw(1600, 900);
	glfw.setTitle("This title is long, long enough to test how glfw manages oversized titles. At this point I dont really know what to write, so let's just bullshiting it ....................................................... is that enough? Well, it depends of the size of the current window. I dont really know how many characters i have to write for a width of 1920. Is it possible to higher the police ? It could save some characters. Ok, im bored, lets check if this title is long enough!");

	//Program for Obj3d (can render Obj3d) with vertex & fragmetns shaders
	Obj3dPG			obj3d_prog(OBJ3D_VS_FILE, OBJ3D_FS_FILE);

	//Create Obj3dBP from .obj files
	Obj3dBP			the42BP("obj3d/42.obj");
	Obj3dBP			cubeBP("obj3d/cube.obj");
	Obj3dBP			teapotBP("obj3d/teapot2.obj");
	Obj3dBP			helmetBP("obj3d/helmet/Helmet.obj");
	Obj3dBP			lamboBP("obj3d/lambo/Lamborginhi_Aventador_OBJ/Lamborghini_Aventador.obj");
	cout << "======" << endl;

	Texture*	texture1 = new Texture("images/lena.bmp");
	Texture*	texture2 = new Texture("images/skybox2.bmp");
	Texture*	texture3 = new Texture("images/skyboxfuck.bmp");
	Texture*	texture4 = new Texture("images/skybox4096.bmp");
	Texture*	texture5 = new Texture("images/skytest.bmp");
	Texture*	texture6 = new Texture("obj3d/helmet/gold_scr.bmp");
	Texture*	texture7 = new Texture("obj3d/lambo/Lamborginhi_Aventador_OBJ/Lamborginhi_Aventador_diffuse.bmp");

	float s = 1.0f;//scale
	//Create Obj3d with the blueprint & by copy
	Obj3d			the42_1(cubeBP, obj3d_prog);//the42BP !
	the42_1.setPos(-4, -2, -2);
	the42_1.setTexture(texture1);
	the42_1._displayTexture = true;
	the42_1.setPolygonMode(GL_FILL);

	Obj3d	the42_2(the42_1);
	//the42_2.setPos(0, 3, -5);
	the42_2.setPos(-4, -2, -2);
	the42_2.setTexture(texture2);
	the42_2._displayTexture = false;
	the42_2._centered = true;
	the42_2.setScale(3.0f, 0.75f, 0.3f);
	the42_2.setPolygonMode(GL_LINE);

	Obj3d			teapot1(teapotBP, obj3d_prog);
	teapot1.setPos(0, 0, 2);
	teapot1.getModelMatrix().setOrder(ROW_MAJOR);
//	teapot1.setRot(0, 90, 0);
	teapot1.setTexture(texture1);
	teapot1._displayTexture = false;
//	teapot1.setPolygonMode(GL_POINT);
	teapot1.setScale(1.5, 2, 0.75);

	Obj3d			cube1(cubeBP, obj3d_prog);
	cube1.setPos(0, -2, 3);
	cube1.setTexture(texture1);
	cube1._displayTexture = false;
//	cube1.setScale(1, 1, 1);

	Obj3d			helmet1(helmetBP, obj3d_prog);
	helmet1.setPos(-10, 0, 0);
	helmet1.setTexture(texture6);
	helmet1._displayTexture = true;
	helmet1._centered = true;
//	helmet1.setPolygonMode(GL_LINE);
	s = 0.4f;
	helmet1.setScale(s,s,s);

	Obj3d			lambo1(lamboBP, obj3d_prog);
	lambo1.setPos(-20, 0, 0);
	lambo1.setTexture(texture7);
	lambo1._displayTexture = true;
	lambo1._centered = true;
	lambo1.setPolygonMode(GL_LINE);
	s = 0.025f;
	lambo1.setScale(s, s, s);

	cout << "Obj3d # : " << Obj3d::instanceAmount << endl;
	cout << endl;

	cout << "GL_MAX_CUBE_MAP_TEXTURE_SIZE " << GL_MAX_CUBE_MAP_TEXTURE_SIZE << endl;
	cout << "GL_MAX_TEXTURE_SIZE " << GL_MAX_TEXTURE_SIZE << endl;

	SkyboxPG	sky_pg(CUBEMAP_VS_FILE, CUBEMAP_FS_FILE);
	Skybox		skybox(*texture5, sky_pg);
	
	vector<Obj3d*>	obj3dList;
	obj3dList.push_back(&the42_1);
	obj3dList.push_back(&the42_2);
	obj3dList.push_back(&teapot1);
	obj3dList.push_back(&cube1);
	obj3dList.push_back(&helmet1);
	obj3dList.push_back(&lambo1);

	
	Cam		cam(glfw);
	cam.setPos(0, 0, 10);
	cam.printProperties();

	cout << "Begin while loop" << endl;
	Fps	fps144(144);
	Fps	fps60(60);
	Fps	fps30(30);
	while (!glfwWindowShouldClose(glfw._window)) {
		if (fps144.wait_for_next_frame()) {
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			renderSkybox(skybox, cam);
			renderObj3d(obj3dList, cam);
			glfwSwapBuffers(glfw._window);
			glfwPollEvents();
			//events(glfw, &gle, &progs[OBJ3D]);
			glfw.updateMouse();//to do before cam's events
			cam.events(glfw, float(fps144.tick));
			float	v1 = 160;
			float	v2 = 100;
			Math::Rotation rot = the42_1.getRot();
			rot.setAsDegree();
			rot.x += v1 * (float)fps144.tick;
			rot.y += v2 * (float)fps144.tick;
			the42_1.setRot(rot);
			helmet1.setRot(rot);

			if (fps60.wait_for_next_frame()) {
				Obj3d*	ptr = &lambo1;
				rot = ptr->getRot();
				rot.setAsDegree();
			//	rot.x += v1 * (float)fps60.tick;
				rot.y += v2/5 * (float)fps60.tick;
				ptr->setRot(rot);

			}
			/*
			*/
		}
	}

	return (EXIT_SUCCESS);
}
