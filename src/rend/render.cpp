// Include standard headers
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <string>
#include <iostream>
#include <thread>
#include <mutex>
#include <chrono>
#include <map>
// Include GLEW
#include <GL/glew.h>
// Include GLFW
#include <GLFW/glfw3.h>
// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtx/norm.hpp>
// Include AntTweakBar
#include <AntTweakBar.h>
// Include copied libraries
#include <common/shader.hpp>
#include <common/texture.hpp>
#include <common/controls.hpp>
#include <common/objloader.hpp>
#include <common/vboindexer.hpp>
#include <common/quaternion_utils.hpp>

#include <render_engine.h>

namespace srs

{
	render_engine *RendEng = new render_engine();

	void initialize_all()
	{

		std::cout << "Render Engine was initialized successfully!\n";
	}
	void start_render()
	{
		RendEng->start();
	}
	void update_data()
	{
	}
	void tmp_func()
	{
		while (!RendEng->GetInitialize())
			std::this_thread::sleep_for(std::chrono::nanoseconds(100));
		RendEng->new_object("cube", 1.0, 2.0, 3.0, 0.0, 0.0, 0.0, 0.0);
		RendEng->new_object("monkey", -1.0, -2.0, -4.0, 0.0, 0.0, 0.0, 0.0);
		RendEng->new_object("cube", -1.0, -2.0, -3.0, 0.0, 0.0, 0.0, 0.0);
		RendEng->new_light("light", 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0);
		RendEng->new_light("light", 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0);
		double x, y, z;
		x = 1.0;
		y = 2.0;
		z = 3.0;
		while (true)
		{
			x += 0.002;
			y -= 0.002;
			z -= 0.002;
			RendEng->update_obj(0, x, y, z, 0.0, 0.0, 0.0, 0.0);
			std::this_thread::sleep_for(std::chrono::nanoseconds(1000000000 / 144));
		}
	}
	void terminate_render()
	{
		std::cout << "Render Engine was terminated successfully!\n";
	}
}