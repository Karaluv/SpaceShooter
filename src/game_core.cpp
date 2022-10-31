// Your First C++ Program

#include <iostream>
#include <thread>
#include <chrono>

#include <render.cpp>
#include <serg_main.cpp>

int main()
{

	srs::start_render();
	{
		// create monkey and cube and several lights in different positions and angels
		srs::create_object("monkey", 0, 0, -1, 0, 0, 0, 1);
		srs::create_object("cube", 4, 0, -1, 0, 0, 0, 1);

		// now I need to create a light
		srs::create_light(8, 3, -2, 1, 1, 1, 50);
		srs::create_light(-8, -3, -2, 1, 1, 1, 50);

		// write some code 144 times per seconds to update camera position, objects position, lights position and color
		for (int i = 0; i < 10000; i++)
		{
			// rotate monkey by sin i around y axis
			srs::update_object(0, 0, 0, -1, 0, sin(double(i) / 100), 0, 1);
			// move cube by sin i around y axis
			srs::update_object(1, 4 + sin(double(i) / 100), 0, -1, 0, 0, 0, 1);

			// change light color by sin i
			srs::update_light(0, 8, 3, -2, sin(double(i) / 100), 0, 0, 100);
			// srs::update_camera(0.0, 0.0, 0.0, 0.0, 0.0, 0.0);
			//  cube move by 0.0001 in x direction
			// srs::update_object(0, 0, 0, -1, 0.0, 0.0, 0.0, 1.0);

			std::this_thread::sleep_for(std::chrono::milliseconds(1));
		}
	}
	srs::stop_render();

	return 0;
}
