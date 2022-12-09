#include <iostream>
#include <optional>
#include <fstream>
#include <stdlib.h>
#include <time.h>
#include "Player_Actions.h"
//#include <matrix.cpp>

unsigned general_rockets_number = 0;

using Type = long double;
Type const PI = 3.14159;
Type const Kc = 500;
Type const Ks = 20;
Type const Kf = 20;

Type get_square(Type number) {
	return number * number;
}

void print_current_state(Type** coords, Type** speeds, Type** powers, unsigned* types, unsigned N);

template <typename T, unsigned N>
void copy_arr(T* data, T* new_data)
{
	for (unsigned k = 0; k < N; ++k)
	{
		data[k] = new_data[k];
	}
}

template <typename T>
void print_arr(T** arr, std::string name, unsigned N, std::ofstream& fin, unsigned time)
{
	if (N > 5) N = 5;
	fin << "Moment of time: " << time << std::endl;
	fin << "Start of output of " << name <<  std::endl;
	for (unsigned k = 0; k < N; ++k)
	{
		fin << k << ": ";
		for (unsigned i = 0; i < 3; ++i)
		{
			fin << arr[k][i] << "  ";
		}
		fin << std::endl;
	}
	fin << "End of output of " << name << std::endl;
}

template <typename T>
void print_arr(T* arr, std::string name, unsigned N, std::ofstream& fin, unsigned time)
{
	if (N > 5) N = 5;
	fin << "Moment of time: " << time << std::endl;
	fin << "Start of output of " << name << std::endl;
	for (unsigned k = 0; k < N; ++k)
	{
		fin << k << " ";
		fin << arr[k] << "  ";
		fin << std::endl;
	}
	fin << "End of output of " << name << std::endl;
}

Type random_distribution(int min, int max)
{
	if (max <= min) {
		std::cout << "error of diapason of random value" << std::endl;
		throw;
	}
	int random_value = static_cast<int>(rand() % (static_cast<unsigned>(max - min))) + min;
	//int random_value = static_cast<int>(rand() % (static_cast<unsigned>(max - min))) + min;
	return static_cast<Type>(random_value);
}

struct Directed_Segment;

class Math_Point;
class Massive_Point;
class Space_Ship;
class Weapon;
class Rocket;
class Object_Management;

struct Directed_Segment {
	Type x;
	Type y;
	Type z;
	Directed_Segment() : x(0), y(0), z(0) {};
	Directed_Segment(Type x, Type y, Type z) : x(x), y(y), z(z) {};
	//Directed_Segment(Type* segm) : x(segm[0]), y(segm[1]), z(segm[2]) {};
	Directed_Segment(Directed_Segment const& src) : Directed_Segment(src.x, src.y, src.z) {};
	Directed_Segment& operator = (Directed_Segment const& src) {
		Directed_Segment buffer = Directed_Segment(src);
		std::swap(this->x, buffer.x);
		std::swap(this->y, buffer.y);
		std::swap(this->z, buffer.z);
		return *this;
	}

	//I don't understand why it's not correct
	/***
	Directed_Segment(Directed_Segment&& src) : x(src.x), y(src.y), z(src.z) {};
	Directed_Segment& operator = (Directed_Segment& src) {
		Directed_Segment buffer(std::move(src));
		std::swap(this->x, buffer.x);
		std::swap(this->y, buffer.y);
		std::swap(this->z, buffer.z);
	};
	***/

	Directed_Segment operator + (Directed_Segment& other) {
		return Directed_Segment(this->x + other.x, this->y + other.y, this->z + other.z);
	}
	Directed_Segment operator - (Directed_Segment& other) {
		return Directed_Segment(this->x - other.x, this->y - other.y, this->z - other.z);
	}
	Directed_Segment operator - () {
		return Directed_Segment(-this->x, -this->y, -this->z);
	}
	Type operator *(Directed_Segment other) {
		return this->x * other.x + this->y * other.y + this->z * other.z;
	}
	Directed_Segment operator *(Type number) {
		return Directed_Segment(number * this->x, number * this->y, number * this->z);
	}
	Directed_Segment operator += (Directed_Segment& other) {
		this->x += other.x;
		this->y += other.y;
		this->z += other.z;
		return *this;
	}
	Directed_Segment operator -= (Directed_Segment& other) {
		this->x -= other.x;
		this->y -= other.y;
		this->z -= other.z;
		return *this;
	}
	Directed_Segment operator *= (Type number) {
		this->x *= number;
		this->y *= number;
		this->z *= number;
		return *this;
	}

	Type get_module() {
		return sqrt(*this * *this);
	}

	Type define_distance(Directed_Segment other) {
		return (*this - other).get_module();
	}

	Directed_Segment multy(Directed_Segment& other) {
		return Directed_Segment(this->y * other.z - this->z * other.y, this->z * other.x - this->x * other.z,
			this->x * other.y - this->y * other.x);
	}

	void print(std::string name)
	{
		std::cout << name << ":  " << x << ' ' << y << ' ' << z << std::endl;
	}

	void print(std::string name, std::ofstream& fin)
	{
		fin << name << ":  " << x << ' ' << y << ' ' << z << std::endl;
	}
};

Directed_Segment set_random_starting_position(int min, int max)
{
	if (min < 0 || max < 0) {
		std::cout << "wrong start diapason of ship position" << std::endl;
		throw;
	}
	unsigned sign = 0;
	Type pos[3];
	for (unsigned k = 0; k < 3; ++k)
	{
		pos[k] = random_distribution(min, max);
		if (rand() % 2 != 0) pos[k] *= -1;
	}
	return Directed_Segment(pos[0], pos[1], pos[2]);
}

Directed_Segment set_random_ball_point(Type R)
{
	Type ro = random_distribution(0, R);
	Type theta = random_distribution(0, 180);
	Type fi = random_distribution(0, 2 * 360);
	return Directed_Segment(ro * sin(theta * PI / 180) * cos(fi * PI / 180),
		ro * sin(theta * PI / 180) * sin(fi * PI / 180), ro * cos(theta * PI / 180));
}

Directed_Segment set_random_sphere_point(Type R)
{
	Type theta = random_distribution(0, 180);
	Type fi = random_distribution(0, 2 * 360);
	return Directed_Segment(R * sin(theta * PI / 180) * cos(fi * PI / 180),
		R * sin(theta * PI / 180) * sin(fi * PI / 180), R * cos(theta * PI / 180));
}


class Math_Point {
protected:
	unsigned number;
	Directed_Segment coord;
	Directed_Segment speed;
	Directed_Segment accel;

public:
	/** Math_Point(double x, double y, double z, double vx, double vy, double vz)
	{
		number = 0;
		coord = new double[3];
		coord[0] = x;
		coord[1] = y;
		coord[2] = z;
		speed = new double[3];
		speed[0] = vx;
		speed[1] = vy;
		speed[2] = vz;
	}; **/
	/** Math_Point(double x, double y, double z) : Math_Point(x, y, z, 0, 0, 0) {}; **/
	Math_Point() : number(0) {};

	void set_coord(Type* arr) {
		coord = Directed_Segment(arr[0] * Kc, arr[1] * Kc, arr[2] * Kc);
	}
	void set_speed(Type* v) {
		speed = Directed_Segment(v[0] * Ks, v[1] * Ks, v[2] * Ks);
	}

	void set_number(unsigned n)
	{
		number = n;
	}

	void set_coord(Directed_Segment const& arr) {
		coord = arr;
	}
	void set_speed(Directed_Segment const& v) {
		speed = v;
	}
	void set_accel(Directed_Segment const& a) {
		accel = a;
	}

	unsigned get_number()
	{
		return number;
	}

	Type* get_list_speed()
	{
		Type* list_speed = new Type[3];
		list_speed[0] = this->speed.x / Ks;
		list_speed[1] = this->speed.y / Ks;
		list_speed[2] = this->speed.z / Ks;
		return list_speed;
	}

	Type* get_list_coord()
	{
		Type* list_coord = new Type[3];
		list_coord[0] = this->coord.x / Kc;
		list_coord[1] = this->coord.y / Kc;
		list_coord[2] = this->coord.z / Kc;
		return list_coord;
	}

	Directed_Segment& get_coord()
	{
		return coord;
	}

	Directed_Segment& get_speed()
	{
		return speed;
	}

	Directed_Segment& get_accel()
	{
		return accel;
	}
};

class Massive_Point : public Math_Point {
protected:
	Type mass;
	Type size;
public:
	Massive_Point() : Math_Point(), mass(1), size(0) {};
	/** Massive_Point(double mass, double x, double y, double z, double vx, double vy, double vz) :
		Math_Point(x, y, z, vx, vy, vz), mass(mass), accel(nullptr) {}; **/

		/** Massive_Point() : Math_Point(), mass(0), accel(nullptr) {}; **/

		/** void move_object(double time, double* f)
		{
			for (unsigned i = 0; i < 3; ++i) {
				accel[i] = f[i] / mass;
				coord[i] += speed[i] * time + accel[i] * time * time / 2;
				speed[i] += accel[i] * time;
			}
		}**/
	void move_object()
	{
		Type const time = 1;
		speed += accel * time;
		coord += speed * time;
	}
};



class Weapon : public Massive_Point {
protected:
	Type const standart_rocket_speed = 200;
	Type const standart_destructive_power = 10;
	unsigned const time_of_life = 200;
	Type start_speed;
	unsigned current_time_of_life;
	unsigned destructive_power;
public:
	Weapon(Type start_speed, unsigned destructive_power) : Massive_Point(), start_speed(start_speed),
		destructive_power(destructive_power), current_time_of_life(0) {};
	Weapon() : Massive_Point(), start_speed(0), destructive_power(0) {};
	void set_parametres(Type start_speed, unsigned destructive_power) {
		this->start_speed = start_speed;
		this->destructive_power = destructive_power;
	}

	// deatructor
	~Weapon() {
		std::cout << "heh";
	};

	unsigned get_destructive_power()
	{
		return destructive_power;
	}

	Type get_start_speed() {
		return start_speed;
	}

	bool check_time_of_life()
	{
		return ( (++current_time_of_life) < time_of_life);
	}
};

class Rocket final : public Weapon {
public:
	Rocket() : Weapon() {
		this->set_parametres(standart_rocket_speed, standart_destructive_power);
		++general_rockets_number;
	}

	~ Rocket()
	{
		--general_rockets_number;
	}
};

class Space_Ship : public Massive_Point {
protected:
	Type const standart_ship_speed = 50;
	Type const standart_ship_accel = 5;
	Type const standart_ship_size = 50;
	Type const detected_distance = 10000; // distance of detection of enemy
	Type const fire_distance = 2000; // distance of starting of shut
	unsigned const standart_recharging_time = 1000;
	unsigned recharging_time;
	unsigned recharging;
	Type const free_length = 1000;
	Type max_speed;
	Type max_engine_power;
	Directed_Segment engine_power;
	Directed_Segment target;
	int const standart_hp = 10;
	int hp;
	unsigned* arsenal;
public:
	Space_Ship() : Massive_Point(), max_speed(0), max_engine_power(0), recharging(0), hp(0), arsenal(nullptr)
	{
		this->set_parametres();
	};

	~Space_Ship()
	{
		if (arsenal != nullptr) delete[] arsenal;
	}

	Space_Ship(Type max_speed, Type max_accel, unsigned hp, Type size, unsigned* arsenal) : Massive_Point(),
		max_speed(max_speed), max_engine_power(max_accel), hp(hp), recharging(0), arsenal(arsenal) {	}

	void virtual set_parametres()
	{
		this->max_speed = standart_ship_speed;
		this->max_engine_power = standart_ship_accel * mass;
		this->size = standart_ship_size;
		this->hp = standart_hp;
		this->recharging_time = standart_recharging_time;
	}

	bool change_direction()
	{
		return coord.define_distance(target) < free_length / 2;
	}

	void define_direction()
	{
		if (this->change_direction()) this->set_target();
	}

	void do_recharging()
	{
		if (recharging > 0) recharging--;
	}

	void set_target()
	{
		target = set_random_sphere_point(this->free_length) + coord;
	}

	void set_target(Directed_Segment new_target)
	{
		target = new_target;
	}

	void set_engine_power()
	{
		engine_power = (target - coord) * (1 / (target - coord).get_module()) * max_engine_power;
	}

	void set_engine_power(Directed_Segment new_engine_power)
	{
		engine_power = new_engine_power;
	}

	Directed_Segment get_target()
	{
		return target;
	}

	Directed_Segment get_engine_power()
	{
		return engine_power;
	}

	Type get_detected_distance()
	{
		return detected_distance;
	}

	Type get_fire_distance()
	{
		return fire_distance;
	}

	void destroy()
	{
		return; // äîïèñàòü
	}

	template<typename T>
	bool get_damage(T force) {
		hp -= (static_cast<int>(force));
		if (hp <= 0)
		{
			return true;
			destroy();
		}
		return false;
	}

	Type get_max_speed()
	{
		return max_speed;
	}

	Type* get_list_power()
	{
		Type* list_power = new Type[3];
		list_power[0] = this->engine_power.x / Kf;
		list_power[1] = this->engine_power.y / Kf;
		list_power[2] = this->engine_power.z / Kf;
		return list_power;
	}

	void shout(unsigned weapon_type, Weapon& bullet, Directed_Segment target, Directed_Segment target_speed) {
		//arsenal[weapon_type] --;
		Directed_Segment calculated_speed = calculate_start_weapon_speed(target, target_speed, bullet.get_start_speed());
		bullet.set_speed(calculated_speed);
		recharging = recharging_time;
		//std::cout << bullet.get_speed().x << bullet.get_speed().y << bullet.get_speed().z << "  setting speed " << std::endl;
	}
	Directed_Segment& calculate_start_weapon_speed(Directed_Segment target, Directed_Segment target_speed,
		Type bullet_speed)
	{
		Type relative_coord[3] = { coord.x - target.x, coord.y - target.y , coord.z - target.z };
		Type goal_speed[3] = { target_speed.x, target_speed.y, target_speed.z };
		unsigned max_number = 0;
		for (unsigned i = 1; i < 3; ++i) {
			if (std::abs(relative_coord[i]) > std::abs(relative_coord[i - 1])) max_number = i;
		}
		if (max_number != 2) {
			std::swap(relative_coord[max_number], relative_coord[2]);
			std::swap(goal_speed[max_number], goal_speed[2]);
		}
		Type x0 = relative_coord[0];
		Type y0 = relative_coord[1];
		Type z0 = relative_coord[2];
		Type A = 1 + (x0 / z0) * (x0 / z0) + (y0 / z0) * (y0 / z0);
		Type B = 2 * (x0 / z0 * (goal_speed[0] - x0 / z0 * goal_speed[2]) + y0 / z0 * (goal_speed[1] - y0 / z0 * goal_speed[2]));
		Type C = get_square(goal_speed[0] - x0 / z0 * goal_speed[2]) + get_square(goal_speed[1] - y0 / z0 * goal_speed[2]) -
			get_square(bullet_speed);
		Type D = B * B - 4 * A * C;
		if (D < 0) return Directed_Segment(bullet_speed, 0, 0);
		Type uz1 = (-B + std::sqrt(D)) / 2 / A;
		Type uz2 = (-B - std::sqrt(D)) / 2 / A;
		Type time1 = z0 / (goal_speed[2] - uz1);
		Type time2 = z0 / (goal_speed[2] - uz2);
		Type uz = uz1;
		if ((time1 < 0) || ((time1 > time2) && (time2 > 0))) { uz = uz2; }
		Type ux = goal_speed[0] - x0 / z0 * goal_speed[2] + x0 / z0 * uz;
		Type uy = goal_speed[1] - y0 / z0 * goal_speed[2] + y0 / z0 * uz;
		Type result_speed[3] = { ux, uy, uz };
		if (max_number != 2) std::swap(result_speed[max_number], result_speed[2]);
		return Directed_Segment(result_speed[0], result_speed[1], result_speed[2]);
	}
};

class Player_Ship : public Space_Ship {
public:
	Player_Ship() : Space_Ship() {};
	void set_parametres() override
	{
		this->max_speed = standart_ship_speed;
		this->max_engine_power = standart_ship_accel * mass;
		this->size = standart_ship_size;
	}
	void shout(unsigned weapon_type, Weapon& bullet, Directed_Segment bullet_speed) {
		//arsenal[weapon_type] --;
		bullet.set_coord(coord);
		bullet.set_speed(bullet_speed);
		recharging = recharging_time;
	}
};

class Object_Management {
private:
	unsigned const max_objects_amount = 10000;
	unsigned const min_start_distance = 1000;
	unsigned const max_start_distance = 10000;
	unsigned const start_ship_number = 20;
	unsigned const amount_types = 4;
	unsigned const ship_type = 1;
	unsigned const rocket_type = 2;
	unsigned const player_ship_type = 3;
	Player_Ship player_ship;
	Space_Ship** buffer_ships;
	Space_Ship** ships;
	Rocket** buffer_rockets;
	bool* real_objects;
	unsigned deleted_objects[50];
	unsigned amount_deleted_obj;
	Rocket** rockets;
	//Math_Point** arr_objects[amount_types];
	unsigned* match_table;
	unsigned* buffer_table;
	unsigned* counter;
	unsigned* buffer_counter;
	unsigned general_number;
public:
	Object_Management(unsigned* types) {
		general_rockets_number = 0;

		srand(time(NULL));
		general_number = 1;
		player_ship.set_number(0);
		types[0] = 3;
		ships = new Space_Ship * [100]{nullptr};
		buffer_ships = new Space_Ship * [100]{ nullptr };
		rockets = new Rocket * [100]{nullptr};
		buffer_rockets = new Rocket * [100]{ nullptr };
		real_objects = new bool[max_objects_amount] { true };
		amount_deleted_obj = 0;
		match_table = new unsigned[max_objects_amount]{0};
		buffer_table = new unsigned[max_objects_amount]{0};
		ships[0] = &player_ship;
		counter = new unsigned[amount_types]{0};
		buffer_counter = new unsigned[amount_types] {0};
		counter[0] = 1;
		for (general_number = 1; general_number <= start_ship_number; ++ general_number) {
			create_object(0, general_number, types);
			ships[general_number]->set_coord(set_random_starting_position(min_start_distance, max_start_distance));
			ships[general_number]->set_speed(set_random_ball_point(ships[general_number]->get_max_speed()));
			ships[general_number]->set_target();
			ships[general_number]->set_engine_power();

			//std::cout << arr_objects[general_number]->get_speed().x << "  "
				//<< arr_objects[general_number]->get_speed().y << "  " << arr_objects[general_number]->get_speed().z << std::endl;
		};
		//std::cout << "real number of rockets:  " << counter[1] << std::endl;

	};
	void print_objects(std::ofstream& fin)
	{
		for (unsigned k = 0; k < start_ship_number; ++k)
		{
			fin << "ship " << k << ':' << std::endl;
			(ships[k]->get_coord()).print("coord", fin);
			(ships[k]->get_speed()).print("speed", fin);
			(ships[k]->get_target()).print("target", fin);
			fin << "target_distance:  " << (ships[k]->get_target() - ships[k]->get_coord()).get_module() << std::endl;
			(ships[k]->get_engine_power()).print("engine_power", fin);
		}
	}

	Space_Ship* find_ship(unsigned number)
	{
		if (number >= general_number) {
			std::cout << "Such object is not existed" << std::endl;
			throw;
		}
		return ships[number % 100];
	}

	Rocket* find_rocket(unsigned number)
	{
		if (number >= general_number) {
			std::cout << "Such object is not existed" << std::endl;
			throw;
		}
		return rockets[match_table[number] % 100];
	}

	void create_object(unsigned type, unsigned number, unsigned* types)
	{
		if (type >= amount_types) {
			std::cout << "Error: No this type of object" << std::endl;
			throw;
		}

		switch (type)
		{
		case 0: {
			ships[counter[type]] = new Space_Ship(); 
			ships[counter[type]]->set_number(number); 
			break; 
		}
		case 1: {
			rockets[counter[type]] = new Rocket(); 
			rockets[counter[type]]->set_number(number); 
			break; 
		}
			  //case 2: {arr_objects[counter[type] + 100 * type] = new Weapon(); break; }
			  //case 3: {arr_objects[counter[type] + 100 * type] = new Space_Ship(); break; }
		}
		match_table[number] = (counter[type] ++) + 100 * type;
		if (counter[type] >= 100) {
			std::cout << "Too many rockets create";
			throw;
		}
		real_objects[number] = true;
		types[number] = type + 1;
	}

	void delete_object(unsigned number)
	{
		real_objects[number] = false;
		if (amount_deleted_obj < 50)
		{
			deleted_objects[amount_deleted_obj++] = number;
		}
	}

	void update_object_list(unsigned* objects_types)
	{
		if (true)
		{
 
		//for testing
		/***
		for (unsigned k = 0; k < amount_types; ++k) 
		{
			std::cout << counter[k] << " General number of objects with type  " << k << std::endl;
		}
		unsigned real_objects_number = 0;
		for (unsigned k = 0; k < max_objects_amount; ++k)
		{
			if (real_objects[k]) ++ real_objects_number;
		}
		std::cout << "All " << real_objects_number << " live objects" << std::endl;
		***/
		//end of testing coord

		unsigned current_number = 0;
 
			for (unsigned current_type = 0; current_type < amount_types; ++current_type)
			{
				for (unsigned current_object = 0; current_object < counter[current_type]; ++current_object)
				{
					if (current_type == 0)
					{
						if (real_objects[ships[current_object]->get_number()])
						{
							buffer_table[current_number] = current_type * 100 + buffer_counter[current_type];
							ships[current_object]->set_number(current_number);
							objects_types[current_number ++] = (current_object == 0) ? 3 : 1;
							std::swap(ships[current_object], buffer_ships[(buffer_counter[current_type]) ++]);
						}
						else
						{
							delete ships[current_object];
							ships[current_object] = nullptr;
						}
					}
					if (current_type == 1)
					{
						if (real_objects[rockets[current_object]->get_number()])
						{
							buffer_table[current_number] = current_type * 100 + (buffer_counter[current_type] ++);
							rockets[current_object]->set_number(current_number);
							objects_types[current_number++] = 2;
							std::swap(rockets[current_object], buffer_rockets[(buffer_counter[current_type]) ++]);
						}
						else
						{
							delete rockets[current_object];
							rockets[current_object] = nullptr;
						}
					}
				}
				counter[current_type] = 0;
			}
			std::swap(buffer_table, match_table);
			std::swap(buffer_counter, counter);
			std::swap(buffer_ships, ships);
			std::swap(buffer_rockets, rockets);
			for (unsigned current_obj = 0; current_obj < current_number; ++current_obj)

			{
				real_objects[current_obj] = true;
			}
			general_number = current_number;

			//for testing
			/***
			for (unsigned k = 0; k < amount_types; ++k)
			{
				std::cout << counter[k] << " General number of objects with type  " << k << std::endl;
			}
			real_objects_number = 0;
			for (unsigned k = 0; k < max_objects_amount; ++k)
			{
				if (real_objects[k]) ++ real_objects_number;
			}
			std::cout << "All " << real_objects_number << " live objects" << std::endl;
			***/
			//end of testing coord
		}
	}

	bool check_necessary_updating_objects_list()
	{
		for (unsigned current_type = 0; current_type < amount_types; ++current_type)
		{
			if (counter[current_type] > 80) return true;
		}
		return false;
	}

	void update_object(Type** coord, Type** speed)
	{
		for (unsigned current_type = 0; current_type < amount_types; ++ current_type)
		{
			switch (current_type)
			{
			case 0:
			{
				for (unsigned current_object = 0; current_object < counter[current_type]; ++current_object)
				{
					unsigned number = ships[current_object]->get_number();
					ships[current_object]->set_coord(coord[number]);
					ships[current_object]->set_speed(speed[number]);
				}
				break;
			}
			case 1:
			{
				for (unsigned current_object = 0; current_object < counter[current_type]; ++current_object)
				{
					unsigned number = rockets[current_object]->get_number();
					rockets[current_object]->set_coord(coord[number]);
					rockets[current_object]->set_speed(speed[number]);
				}
				break;
			}
			}
		}
	}

	void process_collisions(unsigned* arr1, unsigned* arr2, unsigned amount_collisions)
	{
		for (unsigned k = 0; k < amount_collisions; ++ k)
		{
			unsigned number1 = match_table[arr1[k]];
			unsigned number2 = match_table[arr2[k]];
			if (number1 > number2)
			{
				std::swap(number1, number2);
				std::swap(arr1[k], arr2[k]);
			}
			//if (number1 == 0)
			if (number1 / 100 == 0 && number2 / 100 == 1)
			{
				if (find_ship(number1)->get_damage((find_rocket(number2))->get_destructive_power()))
					delete_object(arr1[k]);
				delete_object(arr2[k]);
			}
		}
	}

	void process_events(unsigned* types, std::ofstream &fout)
	{
		for (unsigned current_type = 0; current_type < amount_types; ++current_type)
		{
			for (unsigned current_object = 0; current_object < counter[current_type]; ++current_object)
			{
				switch (current_type) {
				case 0:
				{
					if (! real_objects[ships[current_object]->get_number()])
					{
						//fout << "Error: the object number " << ships[current_object]->get_number() << " not exist" << std::endl;
						break;
					}

					if (current_object == 0)
					{
						ships[current_object]->do_recharging();
						continue;
					}
					//fout << "The ship number " << current_object << ":" << std::endl;
					//fout << "Distance to player_ship " << player_ship.get_coord().define_distance(ships[current_object]->get_coord())
						//<< ":" << std::endl;
					if (player_ship.get_coord().define_distance(ships[current_object]->get_coord()) <
						ships[current_object]->get_detected_distance())
					{
						//fout << "The ship number " << current_object << " detected player ship" << std::endl;
						ships[current_object]->set_target(player_ship.get_coord());
						if (player_ship.get_coord().define_distance(ships[current_object]->get_coord()) <
							ships[current_object]->get_fire_distance())
						{
							//fout << "The ship number " << current_object << " attack player ship" << std::endl;
							create_object(1, general_number ++, types - 1);

							//for tecting
							/*** 
							Rocket* new_rocket = this->find_rocket(general_number - 1);
							Directed_Segment target_coord = player_ship.get_coord();
							Directed_Segment target_speed = player_ship.get_speed();
							***/
							//end of the code for testing

							ships[current_object]->shout(1, *(this->find_rocket(general_number - 1)),
								player_ship.get_coord(), player_ship.get_speed());
						}
					}
					else ships[current_object]->define_direction();
					ships[current_object]->do_recharging();
					ships[current_object]->set_engine_power();
					if (ships[current_object]->get_speed().get_module() >= ships[current_object]->get_max_speed())
					{
						if (ships[current_object]->get_speed() * ships[current_object]->get_engine_power() > 0)
						{
							ships[current_object]->set_engine_power(ships[current_object]->get_engine_power() -
								ships[current_object]->get_speed() *
								(ships[current_object]->get_engine_power() *
									ships[current_object]->get_speed() /
									ships[current_object]->get_speed().get_module()));
						}
					}
					break;
				}
				case 1:
					if (! rockets[current_object]->check_time_of_life())
					{
						delete_object(rockets[current_object]->get_number());
					}
				}
			}
		}
	}

	void do_player_actions(Player_Actions &player_actions, unsigned* objects_types)
	{
		if (player_actions.shout)
		{
			create_object(1, general_number ++, objects_types);
			player_ship.shout(1, *rockets[counter[1] - 1],
				Directed_Segment(player_actions.weapon_speed[0], player_actions.weapon_speed[1], player_actions.weapon_speed[2]));
		}
	}

	void send_changes(Type** coords, Type** speeds, Type** engine_powers, unsigned* types, unsigned& current_objects_amount)
	{
		for (unsigned current_type = 0; current_type < amount_types; ++current_type)
		{
			switch (current_type)
			{
			case 0:
			{
				for (unsigned current_object = 0; current_object < counter[current_type]; ++current_object)
				{

					unsigned number = ships[current_object]->get_number();
					coords[number] = ships[current_object]->get_list_coord();
					speeds[number] = ships[current_object]->get_list_speed();
					engine_powers[number] = ships[current_object]->get_list_power();
				}
				break;
			}
			case 1:
			{
				for (unsigned current_object = 0; current_object < counter[current_type]; ++current_object)
				{
					unsigned number = rockets[current_object]->get_number();
					coords[number] = rockets[current_object]->get_list_coord();
					speeds[number] = rockets[current_object]->get_list_speed();
				}
				break;
			}
			}
		}
		for (unsigned k = 0; k < amount_deleted_obj; ++k)
		{
			types[deleted_objects[k]] = 0;
		}
		current_objects_amount = general_number;
	}

	void get_start_data(Type** coords, Type** speeds, Type** engine_powers, unsigned* types, unsigned& current_objects_amount)
	{
		send_changes(coords, speeds, engine_powers, types, current_objects_amount);
	}

	void launch_cycle(Type** coords, Type** speeds, Type** engine_power,
		unsigned amount_collisions, unsigned* arr1, unsigned* arr2,
		unsigned* type_objects, unsigned& current_objects_amount, Player_Actions &player_actions)
	{
		if (counter[1] != general_rockets_number) 
		{
			std::cout << "Unmatching of the decklaredand fact rockets number " << std::endl;
			throw;
		}
		//print_current_state(coords, speeds, engine_power, type_objects, current_objects_amount);
		update_object(coords, speeds);
		do_player_actions(player_actions, type_objects);
		std::ofstream fout("cords.txt", std::ios::app);
		//if (fout.is_open()) print_arr<Type>(coords, "cords", current_objects_amount, fout, 0);
		//fout.close();
		//for testing
		//fout << "collisions:" << std::endl;
		//for (unsigned k = 0; k < amount_collisions; ++k)
		//{
			//fout << arr1[k] << "  " << arr2[k] << std::endl;
		//}
		//end the code for testing
		process_collisions(arr1, arr2, amount_collisions);
		if (check_necessary_updating_objects_list())
		{
			update_object_list(type_objects);
		}
		process_events(type_objects, fout);
		send_changes(coords, speeds, engine_power, type_objects, current_objects_amount);
		//fout.open("cords.txt", std::ios::app);
		//if (fout.is_open()) print_arr<Type>(engine_power, "forces", current_objects_amount, fout, 0);
		fout.close();
	}

};


void print_current_state(Type** coords, Type** speeds, Type** powers, unsigned* types, unsigned N)
{
	std::ofstream fin("test.txt", std::ios::app);
	for (unsigned k = 0; k < N; ++k)
	{
		fin << "Object k:" << std::endl;
		switch (types[k])
		{
		case 0: {std::cout << "This is not existing object" << std::endl; break; }
		case 1: {std::cout << "Type: Space_Ship" << std::endl; break; }
		case 2: {std::cout << "Type: Rocket" << std::endl; break; }
		}
		fin << "coords:  ";
		for (unsigned i = 0; i < 3; ++i)
		{
			std::cout << coords[k][i] << " ";
		}
		fin << std::endl;
		fin << "speeds:  ";
		for (unsigned i = 0; i < 3; ++i)
		{
			fin << speeds[k][i] << " ";
		}
		fin << std::endl;
		fin << "engine_powers:  ";
		for (unsigned i = 0; i < 3; ++i)
		{
			fin << powers[k][i] << " ";
		}
		fin << std::endl;
	}
	fin.close();
}


 
