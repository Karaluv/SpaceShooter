// Include standard headers
#include <string>
#include <iostream>
#include <thread>
#include <chrono>

#include <render_engine.cpp>

namespace srs

{
	// create class render_engine (singleton)
	render_engine* RendEng = render_engine::GetInstance();
	

	
	
	
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
	void update_camera(float x, float y, float z, float pitch, float yaw, float roll)
	{
		RendEng->update_camera(x, y, z, pitch, yaw, roll, 0);
	}
	// void which updates object position and rotation using doubles and object id
	void update_object(int id, float x, float y, float z, float rx, float ry, float rz, float rw)
	{
		RendEng->update_object(id, x, y, z, rx, ry, rz, rw);
	}
	// void which updates light position and color with power and light id
	void update_light(int id, float x, float y, float z, float r, float g, float b, float p)
	{
		RendEng->update_light(id, x, y, z, r, g, b, p);
	}

	// void which creates new object with given parameters
	void create_object(std::string path, float x, float y, float z, float rx, float ry, float rz, float rw)
	{
		RendEng->create_object(path, x, y, z, rx, ry, rz, rw);
	}
	// void which creates new light with given parameters
	void create_light(float x, float y, float z, float r, float g, float b, float p)
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