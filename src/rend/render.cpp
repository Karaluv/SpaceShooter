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

	void start_render()
	{
		RendEng->start();
		// add loop which will check if render is initialized
		while (!RendEng->GetInitialize())
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(1));
		}
	}

	// void which updates camera position, angel, rotation from doubles
	void update_camera(double x, double y, double z, double pitch, double yaw, double roll)
	{
		RendEng->update_camera(x, y, z, pitch, yaw, roll, 0);
	}
	// void which updates object position and rotation using doubles and object id
	void update_object(int id, double x, double y, double z, double rx, double ry, double rz, double rw)
	{
		RendEng->update_object(id, x, y, z, rx, ry, rz, rw);
	}
	// void which updates light position and color with power and light id
	void update_light(int id, double x, double y, double z, double r, double g, double b, double p)
	{
		RendEng->update_light(id, x, y, z, r, g, b, p);
	}

	// void which creates new object with given parameters
	void create_object(std::string path, double x, double y, double z, double rx, double ry, double rz, double rw)
	{
		RendEng->create_object(path, x, y, z, rx, ry, rz, rw);
	}
	// void which creates new light with given parameters
	void create_light(double x, double y, double z, double r, double g, double b, double p)
	{
		RendEng->create_light("test", x, y, z, r, g, b, p);
	}
	// void which deletes object with given id
	void delete_object(int id)
	{
		RendEng->delete_object(id);
	}
	// void which deletes light with given id
	void delete_light(int id)
	{
		RendEng->delete_light(id);
	}
	void update_data()
	{
	}

	void stop_render()
	{
		RendEng->stop();
	}
}