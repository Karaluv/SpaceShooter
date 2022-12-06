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
#include "Player_Actions.h"

struct Player {
	unsigned int tip;
	Type cord;
	Type speed;
	Type force;
	unsigned hp;
};


struct element {
	unsigned int tip;
	Type cord;
	Type speed;
	Type force;

	element(unsigned int tip, Type cord, Type speed, Type force) : tip(tip), cord(cord), speed(speed), force(force) {}
	element() : tip(0), cord(0), speed(0), force(0) {}

	//rule of five
	element(const element& other) : tip(other.tip), cord(other.cord), speed(other.speed), force(other.force) {}
	element(element&& other) noexcept : tip(other.tip), cord(other.cord), speed(other.speed), force(other.force) {}
	element& operator=(const element& other) { tip = other.tip; cord = other.cord; speed = other.speed; force = other.force; return *this; }
	element& operator=(element&& other) noexcept { tip = other.tip; cord = other.cord; speed = other.speed; force = other.force; return *this; }
	~element() {}
};



int main()
{

	// define lld type as long long double
	typedef long double lld;

	


	
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	srs::start_render();
	{


		double x = 0;
		double y = 0;
		double z = 0;

		double ax = 0;
		double ay = 0;
		double az = 0;

		double pitch = 0;
		double yaw = 1;
		double roll = 0;



		//for simple test

		lld basic_coord = 0;
		lld basic_speed = 1;
		lld basic_accel = 0;
		lld dt = 0.01;

		matrix<lld, 3> tensor1(3, 0, 0, 0, 4, 0, 0, 0, 6);
		directed_segment <lld> r1(0, 0, 0);
		directed_segment <lld> v1(0, 0, 0);
		directed_segment <lld> angle1(0.1, 0.1, 0.1);
		directed_segment <lld> w1(0.1, 20, 0);
		lld size1 = 1;
		lld m1 = 1;
		directed_segment <lld> nul(0, 0, 0);
		Body<lld> body1_Monki(m1, tensor1, r1, v1, angle1, w1, size1);



		//Array of bodies, there always 10000 of them (some are living, other - dead)
		lld** SPEED = new lld * [10000];
		lld** FORCE = new lld * [10000];
		lld** CORD = new lld * [10000];
		unsigned int* TIP = new unsigned int[10000];
		unsigned int* IS_COLLIDED = new unsigned int[10000];
		for (int i = 0; i < 10000; i++) {
			SPEED[i] = new lld[3];
			FORCE[i] = new lld[3];
			CORD[i] = new lld[3];
			TIP[i] = 0;
			IS_COLLIDED[i] = 0;
			for (int j = 0; j < 3; j++) {
				SPEED[i][j] = 0;
				FORCE[i][j] = 0;
				CORD[i][j] = 0;
			}
		}
		unsigned int collision_count = 0;
		unsigned int* R1 = new unsigned int [10000];
		unsigned int* R2 = new unsigned int [10000];
		unsigned int current_number = 0;
		directed_segment<lld> null_moment(0, 0, 0);

		//Array of bodies
		Body<lld>* bodies = new Body<lld>[10000];
		directed_segment<lld> cord;
		directed_segment<lld> speed;
		directed_segment<lld> force;
		for (int i = 0; i < 10000; i++) {
			if (TIP[i] == 0) {
				bodies[i] = Body<lld>(0, tensor1, nul, nul, nul, nul, 0);
			}
			else {
				cord[0] = CORD[i][0];
				cord[1] = CORD[i][1];
				cord[2] = CORD[i][2];
				speed[0] = SPEED[i][0];
				speed[1] = SPEED[i][1];
				speed[2] = SPEED[i][2];
				bodies[i] = Body<lld>(1, tensor1, cord, speed, angle1, w1, size1);
			}
		}
		
		



		// test meanings of starting parametres

		Player_Actions player_actions;
		Object_Management Manager(TIP);
		Manager.get_start_data(CORD, SPEED, FORCE, TIP, current_number);
		
		//Manager.create_object(3, 1); //creation of spaceship
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
		srs::create_object("cube", 4, 0, -1, 0, 0, 0, 1);

		// now I need to create a light
		srs::create_light(8, 3, -2, 1, 1, 1, 50);
		srs::create_light(-8, -3, -2, 1, 1, 1, 50);

		// write some code 144 times per seconds to update camera position, objects position, lights position and color
		for (int i = 0; i < 10000; i++)
		{
			//Manager.update_object(data, 1);
			// rotate monkey by sin i around y axis

			srs::update_object(0, 0, 0, -1, 0, sin(float(i) / 100), 0, 1);

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
			ax = mouse_pos.first/1000;
			ay = mouse_pos.second/1000;

			

			// camera update
			srs::update_camera(x, y, z, ax, ay, roll);
			// proccessing every object
			Manager.launch_cycle(CORD, SPEED, FORCE, collision_count, R1, R2, TIP, current_number);
			
			for (int i1 = 0; i1 < 10000; ++i1) {
				IS_COLLIDED[i1] = 0;
				R1[i1] = 0;
				R2[i1] = 0;
				collision_count = 0;
			}
			for (int i1 = 0; i1 < 10000; ++i1) {
				
				if (TIP[i1] != 0) {
					bodies[i1].update_angle(dt);
					bodies[i1].update_w(dt, null_moment);
					bodies[i1].update_velocity(dt, force);
					bodies[i1].update_position(dt);

					CORD[i1][0] = bodies[i1].r[0];
					CORD[i1][1] = bodies[i1].r[1];
					CORD[i1][2] = bodies[i1].r[2];
					SPEED[i1][0] = bodies[i1].v[0];
					SPEED[i1][1] = bodies[i1].v[1];
					SPEED[i1][2] = bodies[i1].v[2];




					for (int j = 0; j < 10000; j++) {
						if (IS_COLLIDED[i1] == 0 && IS_COLLIDED[j] == 0 && TIP[j] != 0) {
							if (bodies[i1].size + bodies[j].size >= (bodies[i1].r - bodies[j].r).length()) {
								IS_COLLIDED[i1] = 1;
								IS_COLLIDED[j] = 1;
								bodies[i1].collision(bodies[j]);
								R1[collision_count] = i1;
								R2[collision_count] = j;
								collision_count++;
							}
						}
					}
				}
				


			}

			// rotate sphere by sin i around y axis
			//srs::update_object(2, 0, 0, -2, 0, pi/2, 0, 0);

			//std::cout << body1_Monki.angle[0];
			//body1_Monki.angle[0], body1_Monki.angle[1], body1_Monki.angle[2]
			srs::update_object(0, 0, -1, -1, body1_Monki.angle[0], body1_Monki.angle[1], body1_Monki.angle[2], 0);
			body1_Monki.update_angle(dt);
			//body1_Monki.update_w(dt, null_moment); null_moment is not defined
			//body1_Monki.update_rotation();
			// print kinetic energy
			std::cout << body1_Monki.get_kinetic_energy() << "\n";

			// move cube by sin i around y axis
			srs::update_object(1, 4 + sin(float(i) / 100), 0, -1, 0, 0, 0, 1);

			// change light color by sin i
			srs::update_light(0, 8, 3, -2, sin(float(i) / 100), 0, 0, 100);
			// srs::update_camera(0.0, 0.0, 0.0, 0.0, 0.0, 0.0);
			//  cube move by 0.0001 in x direction
			// srs::update_object(0, 0, 0, -1, 0.0, 0.0, 0.0, 1.0);

			std::this_thread::sleep_for(std::chrono::milliseconds(1));
		}
	}
	srs::stop_render();

	return 0;

}
