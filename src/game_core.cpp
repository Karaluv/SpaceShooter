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
#include <fstream>

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

	matrix<lld, 3> tensor1(3, 0, 0, 0, 4, 0, 0, 0, 6);
	directed_segment <lld> r1(0, 0, 0);
	directed_segment <lld> v1(0, 0, 0);
	directed_segment <lld> angle1(0.1, 0.1, 0.1);
	directed_segment <lld> w1(1, 20, 0);
	lld size1 = 1;
	lld m1 = 1;
	directed_segment <lld> nul(0.001, 0.001, 0.001);
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
	unsigned int* R1 = new unsigned int[10000];
	unsigned int* R2 = new unsigned int[10000];
	unsigned int current_number = 0;
	directed_segment<lld> null_moment(0, 0, 0);

	//Array of bodies
	Body<lld>* bodies = new Body<lld>[10000];
	directed_segment<lld> cord;
	directed_segment<lld> speed;
	directed_segment<lld> force;
	for (int i = 0; i < 10000; i++) {
		if (TIP[i] == 0) {
			bodies[i] = Body<lld>(1, tensor1, nul, nul, angle1, w1, 0);
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


	std::ofstream fout0("cords.txt");
	fout0.close();
	std::ofstream fin;
	fin.open("arrays.txt");


	
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	srs::start_render();
		
		srs::init();

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



		
		
		float* x_coords = new float[500];
		float* y_coords = new float[500];
		float* z_coords = new float[500];

		float* ps = new float[500];
		float* qs = new float[500];
		float* rs = new float[500];

		unsigned int* types = new unsigned int[500];



		//Array of bodies, there always 10000 of them (some are living, other - dead)

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

		//Array of bodies
		
		for (int i = 0; i < 10000; i++) {
			if (TIP[i] == 0) {
				bodies[i] = Body<lld>(0, tensor1, nul, nul, angle1, w1, 0);
			}
			else {
				cord[0] = CORD[i][0];
				cord[1] = CORD[i][1];
				cord[2] = CORD[i][2];
				speed[0] = SPEED[i][0];
				speed[1] = SPEED[i][1];
				speed[2] = SPEED[i][2];
				bodies[i] = Body<lld>(1, tensor1, cord, speed, angle1, w1, size1);
				//std::cout << bodies[i].angle << "ln";
			}
		}



		// test meanings of starting parametres
		Player_Actions player_actions;
		Object_Management Manager(TIP);
		Manager.get_start_data(CORD, SPEED, FORCE, TIP, current_number);

		//print_arr<Type>(CORD, "coords", current_number, fin, 0);
		//print_arr<Type>(SPEED, "coords", current_number, fin, 0);
		
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

		srs::create_object("skybox", 0, 0, 0, 0, 0, 0, 1);


		// now I need to create a light
		srs::create_light(1000, 1000, 1000, 1, 0.7, 0.1, 5000000);
		//srs::create_light(-8, -3, -2, 1, 1, 1, 50);

		// write some code 144 times per seconds to update camera position, objects position, lights position and color
		for (int cycle = 0; cycle < 100000; cycle++)
		{
			{
				
				Manager.launch_cycle(CORD, SPEED, FORCE, collision_count, R1, R2, TIP, current_number, player_actions);

				if (true)
				{
					for (int i = 0; i < 10000; i++) {
						if (TIP[i] == 0) {
							bodies[i] = Body<lld>(1, tensor1, nul, nul, angle1, w1, 0);
						}
						else {
							cord[0] = CORD[i][0];
							cord[1] = CORD[i][1];
							cord[2] = CORD[i][2];
							speed[0] = SPEED[i][0];
							speed[1] = SPEED[i][1];
							speed[2] = SPEED[i][2];
							bodies[i].m = 1;
							bodies[i].tensor = tensor1;
							bodies[i].r = cord;
							bodies[i].v = speed;
							bodies[i].size = size1;
							//std::cout << bodies[i].angle[0] << "ln";
							//std::cout << bodies[i].r[0] << "ln";
							//bodies[i] = Body<lld>(1, tensor1, cord, speed, angle1, w1, size1);
						}
					}
				}

				// testing code (please not delete)
				/*
				if (cycle % 100 == 0) {
					std::cout << general_rockets_number << std::endl;
					std::cout << std::endl;
					//print_arr<Type>(CORD, "coords", current_number, fin, cycle);
					//print_arr<Type>(SPEED, "speeds", current_number, fin, cycle);
				}
				*/
				// end of the testing code

				if (true)
				{
					for (int i1 = 0; i1 < 10000; ++i1) {
						IS_COLLIDED[i1] = 0;
						R1[i1] = 0;
						R2[i1] = 0;
						collision_count = 0;
					}
					for (int i1 = 0; i1 < 10000; ++i1) {

						if (TIP[i1] != 0) {
							force[0] = FORCE[i1][0];
							force[1] = FORCE[i1][1];
							force[2] = FORCE[i1][2];
							try{
								bodies[i1].update_angle(dt);
								bodies[i1].update_w(dt, null_moment);
								bodies[i1].update_velocity(dt, force);
								bodies[i1].update_position(dt);
							}
							catch (std::invalid_argument& e) {
								std::cout << e.what() << std::endl;
							}
							


							//std::cout << SPEED[i1][0] <<" ";
							CORD[i1][0] = bodies[i1].r[0];
							CORD[i1][1] = bodies[i1].r[1];
							CORD[i1][2] = bodies[i1].r[2];
							SPEED[i1][0] = bodies[i1].v[0];
							SPEED[i1][1] = bodies[i1].v[1];
							SPEED[i1][2] = bodies[i1].v[2];
							//std::cout << SPEED[i1][0] << "\n";



							for (int j = 0; j < 10000; j++) {
								if (IS_COLLIDED[i1] == 0 && IS_COLLIDED[j] == 0 && TIP[j] != 0 && i1 != j) {
									if (bodies[i1].size + bodies[j].size >= (bodies[i1].r - bodies[j].r).length()) {
										IS_COLLIDED[i1] = 1;
										IS_COLLIDED[j] = 1;
										//try and catch
										try {
											if (TIP[i1] == TIP[j]) {
												bodies[i1].collision(bodies[j]);
											}
										}
										catch (const std::invalid_argument& e) {
											std::cout << e.what() << "\n";
										}
										R1[collision_count] = i1;
										R2[collision_count] = j;
										collision_count++;
									}
								}
							}
						}
					}
					//body1_Monki.update_angle(dt);
					// copy from bodies to arrays
					for (int i = 1; i < 500; ++i)
					{
						x_coords[i - 1] = bodies[i].r[0];
						y_coords[i - 1] = bodies[i].r[1];
						z_coords[i - 1] = bodies[i].r[2];

						ps[i - 1] = bodies[i].angle[0];
						qs[i - 1] = bodies[i].angle[1];
						rs[i - 1] = bodies[i].angle[2];

						types[i - 1] = TIP[i];
					}

				}
			}
			if (player_actions.hp <= 0) {
				std::cout << "gameover";
				break;
			}
			
			std::map<char, bool> inputs = srs::get_inputs();
			
			if (inputs['w'])
			{
				x += 0.1 * sin(ax);
				z += 0.1 * cos(ax);
			}
			if (inputs['s']) {
				x -= 0.1 * sin(ax);
				z -= 0.1 * cos(ax);
			}
			if (inputs['a']) {
				z -= 0.1 * sin(ax);
				x += 0.1 * cos(ax);
			}
			if (inputs['d']) {
				z += 0.1 * sin(ax);
				x -= 0.1 * cos(ax);
			}
			if (inputs[' '])
				y += 0.1;
			if (inputs['c'])
				y -= 0.1;
			if (inputs['e'])
				roll += 0.1;
			if (inputs['q'])
				roll -= 0.1;
			player_actions.shout = false;
			if (inputs['l'])
				player_actions.shout = true;

			std::pair<double, double> mouse_pos = srs::get_mouse();

			
			ax = -mouse_pos.first/1000;
			ay = -mouse_pos.second/1000;

			bodies[0].r[0] = x;
			bodies[0].r[1] = y;
			bodies[0].r[2] = z;

			bodies[0].angle[0] = ax;
			bodies[0].angle[1] = ay;
			bodies[0].angle[2] = az;

			CORD[0][0] = x;
			CORD[0][1] = y;
			CORD[0][2] = z;


			

			srs::update_camera(x, y, z, ax, ay, roll);

			srs::sync_changes(types, x_coords, y_coords, z_coords, ps, qs, rs);
			
			std::this_thread::sleep_for(std::chrono::milliseconds(1));
		}
	
		//srs::stop_render();
		fin.close();
		srs::stop_render();

		// clear memory
		{
			for (int i = 0; i < 500; ++i)
			{
				delete[] CORD[i];
				delete[] SPEED[i];
				delete[] FORCE[i];
			}
			delete[] CORD;
			delete[] SPEED;
			delete[] FORCE;

			delete[] IS_COLLIDED;
			delete[] TIP;
			delete[] R1;
			delete[] R2;
			delete[] bodies;
			//delete[] Force;
			delete[] x_coords;
			delete[] y_coords;
			delete[] z_coords;

			delete[] ps;
			delete[] qs;
			delete[] rs;

			delete[] types;
		}

	return 0;

}