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

		//for simple test
		double basic_coord = 0;
		double basic_speed = 1;
		double basic_accel = 0;
		double dt = 0.01;
		
		matrix<float, 3> tensor1(1, 0, 0, 0, 1, 0, 0, 0, 1);
		directed_segment <float> r1(0, 0, 0);
		directed_segment <float> v1(0, 0, 0);
		directed_segment <float> angle1(0.001, 0.001, 0.001);
		//std::cout << "\n" << angle1[0] << " " << angle1[1] << " " << angle1[2] << "\n";
		directed_segment <float> w1(1, 1, 0);
		float size1 = 1;
		float m1 = 1;
		Body<float> body1_Monki(m1, tensor1, r1, v1, angle1, w1, size1);
		
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
		srs::create_object("monkey", 0, 0, -1, 0, 0, 0, 1);
		srs::create_object("monkey", 4, 0, -1, 0, 0, 0, 1);
		srs::create_object("monkey", -4, 0, -1, 0, 0, 0, 1);
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

			//std::cout << body1_Monki.angle[0];
			//body1_Monki.angle[0], body1_Monki.angle[1], body1_Monki.angle[2]
			srs::update_object(0, 0, -1, -1, 0, body1_Monki.angle[0], body1_Monki.angle[1], body1_Monki.angle[2]);
			body1_Monki.update_w(dt);
			body1_Monki.update_angle(dt);
			
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
