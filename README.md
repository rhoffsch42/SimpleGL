# OpenGL
OpenGL 4


[Video demo](https://youtu.be/pqTejezGYrQ?t=7). This scene is 170 lines of code.

- Example of a scene with a car and a skybox:
```c++
	Glfw		glfw(1600, 900); // screen size
	Obj3dPG		obj3d_prog("obj3d.vs.glsl", "obj3d.fs.glsl"); // vertex shader and fragment shader

	Obj3dBP		lamboBP("Lamborghini.obj", true);
	Texture*	texture1 = new Texture("Lamborginhi.bmp");

	Obj3d		lambo1(lamboBP, obj3d_prog);
	lambo1.local.setPos(0, 0, 0);
	lambo1.setTexture(texture1);
	lambo1.displayTexture = true;
	lambo1.local.centered = true;

	list<Obj3d*>	obj3dList;
	obj3dList.push_back(&lambo1);

	SkyboxPG	sky_pg(CUBEMAP_VS_FILE, CUBEMAP_FS_FILE); // vertex shader and fragment shader for a cubemap
	Texture*	texture2 = new Texture("skybox.bmp");
	Skybox		skybox(*texture2, sky_pg);
	
	Cam		cam(glfw);
	cam.local.centered = false;
	cam.local.setPos(0, 0, 10);
	cam.lockedMovement = false;
	cam.lockedOrientation = false;
	
	Fps	fps144(144);
	Fps* defaultFps = &fps144;
	
	while (!glfwWindowShouldClose(glfw._window)) {
		if (defaultFps->wait_for_next_frame()) {
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
```
