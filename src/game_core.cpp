#define _CRTDBG_MAP_ALLOC

#define enable_testing

#include <stdexcept>
#include <limits>
#include <crtdbg.h>
#include <stdlib.h>

#include <chrono>
#include <iostream>
#include <thread>

#include <render.cpp>
#include <serg_main.cpp>
#include <andrei_main.cpp>

#include <mynum.cpp>
#include <MyDict.cpp>

#include "globals.hpp"


int main()
{

	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	srs::start_render();
	{

		double x = 0;
		double y = 0;
		double z = 1;

		double ax = 0;
		double ay = 0;
		double az = 0;

		double pitch = 0;
		double yaw = 1;
		double roll = 0;
		

		//for simple test
		double basic_coord = 0;
		double basic_speed = 1;
		double basic_accel = 0;
		double dt = 0.01;
		// test meanings of starting parametres

		Object_Management Manager;
		Manager.create_object(3, 1); //creation of spaceship
		double*** data = new double**[3];
		for (unsigned k = 0; k < 3; ++ k)
		{
			data[k] = new double* [1000];
			for (unsigned j = 0; j < 1000; ++j) {
				data[k][j] = new double[3];
			}
		}
		for (unsigned i = 0; i < 3; ++i) {
			data[0][0][i] = basic_coord;
		}
		data[1][0][0] = basic_speed;
		data[1][0][1] = 0;
		data[1][0][2] = 0;

		//end of the code for the simple test

		// create monkey and cube and several lights in different positions and angels
		// 6 heads around 0,0,0 positon
		//srs::create_object("monkey", 0, 0, -1, 0, 0, 0, 0);
		
		
		srs::create_object("monkey", 4, 0, 0, 0, 0, 0, 1);
		srs::create_object("monkey", 0, 4, 0, 0, 0, 0, 1);
		srs::create_object("monkey", -4, 0, 0, 0, 0, 0, 1);
		srs::create_object("monkey", 0, -4, 0, 0, 0, 0, 1);
		srs::create_object("monkey", 0, 0, 4, 0, 0, 0, 1);
		srs::create_object("monkey", 0, 0, -4, 0, 0, 0, 1);

		
		
		
		// for sphere
		//srs::create_object("sphere", 0, 0, -1, 0, 0, 0, 1);

		// now I need to create a light
		srs::create_light(8, 3, -2, 1, 1, 1, 50);
		srs::create_light(-8, -3, -2, 1, 1, 1, 50);

		// write some code 144 times per seconds to update camera position, objects position, lights position and color
		for (int i = 0; i < 10000; i++)
		{
			//Manager.update_object(data, 1);
			// rotate monkey by sin i around y axis
			float pi = 3.14159265359;

			// read angles floats
			float x_angle = 0;
			float y_angle = 0;
			float z_angle = 0;
			
			//std::cout << "Enter x angle: ";
			//std::cin >> x_angle;
			//std::cout << "Enter y angle: ";
			//std::cin >> y_angle;
			//std::cout << "Enter z angle: ";
			//std::cin >> z_angle;
			
			//srs::update_object(1, 0, 0, -1, x_angle*pi/180, y_angle*pi/180, z_angle*pi/180, 0);
			//srs::update_object(2, 4, 0, -1, x_angle * pi / 180, y_angle * pi / 180, 0, 0);
			//srs::update_object(0, -4, 0, -1, x_angle * pi / 180, 0, 0, 0);

			// get inputs map from render
			std::map<char, bool> inputs = srs::get_inputs();
			
			// update camera position
			if (inputs['w'])
				x += 0.1;
			if (inputs['s'])
				x -= 0.1;
			if (inputs['a'])
				z -= 0.1;
			if (inputs['d'])
				z += 0.1;
			// ctrl space
			if (inputs[' '])
				y += 0.1;
			if (inputs['c'])
				y -= 0.1;

			// e
			if (inputs['e'])
				roll += 0.1;
			
			// q
			if (inputs['q'])
				roll -= 0.1;

			// get mouse position
			std::pair<double, double> mouse_pos = srs::get_mouse();
			
			// update camera rotation
			//yaw += mouse_pos.first;
			//ax += mouse_pos.second;
			
			// mouse angle
			az = mouse_pos.first/100;
			ay = mouse_pos.second/100;

			

			// camera update
			srs::update_camera(x, y, z, ax, ay, az, pitch, yaw, roll);

			// rotate sphere by sin i around y axis
			//srs::update_object(2, 0, 0, -2, 0, pi/2, 0, 0);
			// move cube by sin i around y axis
			//srs::update_object(1, 4 + sin(float(i) / 100), 0, -1, 0, 0, 0, 1);

			// change light color by sin i
			//srs::update_light(0, 8, 3, -2, sin(float(i) / 100), 0, 0, 100);
			// srs::update_camera(0.0, 0.0, 0.0, 0.0, 0.0, 0.0);
			//  cube move by 0.0001 in x direction
			//srs::update_object(0, 0, 0, -1, 0.0, 0.0, 0.0, 1.0);

			std::this_thread::sleep_for(std::chrono::milliseconds(1));
		}
	}
	srs::stop_render();

	return 0;

}
