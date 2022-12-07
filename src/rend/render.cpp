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
	

	// because in other classes normal arrays are used and vectors in render
	// i got to map indeces

	unsigned* is_alive_previous = new unsigned[500];
	// fill with 0
	
	void init()
	{
	for (int i = 0; i < 500; i++)
	{
		is_alive_previous[i] = 0;
	}

	}
	// map int int
	std::map<int, int> indeces;
	
	std::map<int, std::string> int_to_name{ 
		{1,"monkey"},
		{2,"cube"}};

	void sync_changes(unsigned * is_alive,
		float * x_coords, float *y_coords, float *z_coords, 
		float *p, float *q, float *r)
	{
		
		for (int i = 0; i < 30; ++i)
		{
			//std::cout << i << ' ' << is_alive[i] << ' ' << is_alive_previous[i]<< ' ' << indeces[i] << std::endl;
			if (is_alive_previous[i] != 0 && is_alive_previous[i] != is_alive[i])
			{
				// delete object
				RendEng->delete_object(indeces[i]);
				
				// move indeces
				for (int j = i; j < 500; ++j)
				{
					indeces[j] = indeces[j]-1;
				}
				
				if (is_alive != 0)
				{
					// add object
					RendEng->create_object(int_to_name[is_alive[i]], x_coords[i], y_coords[i], z_coords[i], p[i], q[i], r[i], 0);
					indeces[i] = RendEng->get_number_of_objects()-1;
				}
			}
			else if (is_alive_previous[i] == 0 && is_alive[i] != 0)
			{
				// add object
				RendEng->create_object(int_to_name[is_alive[i]], x_coords[i], y_coords[i], z_coords[i], p[i], q[i], r[i], 0);
				indeces[i] = RendEng->get_number_of_objects()-1;
			}
			else if (is_alive_previous[i] != 0 && is_alive[i] != 0)
			{
				// update object
				RendEng->update_object(indeces[i], x_coords[i], y_coords[i], z_coords[i], p[i], q[i], r[i], 0);
			}

			
		}

		for (int i = 0; i < 500; ++i)
		{
			is_alive_previous[i] = is_alive[i];
		}
		
	}
	
	
	void start_render()
	{
		RendEng->start();
		// add loop which will check if render is initialized
		while (!RendEng->GetInitialize())
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(1));
		}
	}

	// get inputs
	std::map<char, bool> get_inputs()
	{
		return RendEng->get_inputs();
	}
	// for mouse x and y
	std::pair<int, int> get_mouse()
	{
		return RendEng->get_mouse();
	}

	// void which updates camera position, angel, rotation from doubles
	void update_camera(float x, float y, float z, float mouse_x, float mouse_y, float roll) 
	{
		RendEng->update_camera(x, y, z, mouse_x, mouse_y, roll);
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