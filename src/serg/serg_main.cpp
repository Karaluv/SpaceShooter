#include <iostream>
#include <optional>
#include <fstream>
#include <stdlib.h>
#include <time.h>
//#include <matrix.cpp>

using Type = double;
Type const PI = 3.14159;

Type get_square(Type number) {
	return number * number;
}

template <typename T, unsigned N>
void copy_arr(T* data, T* new_data)
{
	for (unsigned k = 0; k < N; ++k)
	{
		data[k] = new_data[k];
	}
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
		coord = Directed_Segment(arr[0], arr[1], arr[2]);
	}
	void set_speed(Type* v) {
		speed = Directed_Segment(v[0], v[1], v[2]);
	}

	void set_accel(Type* a) {
		speed = Directed_Segment(a[0], a[1], a[2]);
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

	Type* get_list_coord()
	{
		Type list_coord[3] = { this->coord.x, this->coord.y, this->coord.z };
		return list_coord;
	}

	Type* get_list_speed()
	{
		Type list_speed[3] = { this->speed.x, this->speed.y, this->speed.z };
		return list_speed;
	}

	Type* get_list_accel()
	{
		Type list_accel[3] = { this->accel.x, this->accel.y, this->accel.z };
		return list_accel;
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
	Type start_speed;
	unsigned destructive_power;
public:
	Weapon(Type start_speed, unsigned destructive_power) : Massive_Point(), start_speed(start_speed),
		destructive_power(destructive_power) {};
	Weapon() : Massive_Point(), start_speed(0), destructive_power(0) {};
	void set_parametres(Type start_speed, unsigned destructive_power) {
		this->start_speed = start_speed;
		this->destructive_power = destructive_power;
	}

	unsigned get_destructive_power()
	{
		return destructive_power;
	}

	Type get_start_speed() {
		return start_speed;
	}
};

class Rocket final : public Weapon {
public:
	Rocket() : Weapon() {
		this->set_parametres(standart_rocket_speed, standart_destructive_power);
	}
};

class Space_Ship : public Massive_Point {
protected:
	Type const standart_ship_speed = 50;
	Type const standart_ship_accel = 5;
	Type const standart_ship_size = 50;
	Type const detected_distance = 1500; // distance of detection of enemy
	Type const fire_distance = 750; // distance of starting of shut
	unsigned const standart_recharging_time = 100;
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
	Space_Ship() : Massive_Point(), max_speed(0), max_engine_power(0), hp(0), arsenal(nullptr)
	{
		this->set_parametres();
	};

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
		return; // дописать
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
};

class Object_Management {
private:
	unsigned const max_objects_amount = 10000;
	unsigned const min_start_distance = 2000;
	unsigned const max_start_distance = 20000;
	unsigned const start_ship_number = 20;
	unsigned const amount_types = 4;
	Space_Ship player_ship;
	Space_Ship** ships;
	bool* real_objects;
	unsigned deleted_objects[10];
	unsigned amount_deleted_obj;
	Rocket** rockets;
	//Math_Point** arr_objects[amount_types];
	unsigned* match_table;
	unsigned* counter;
	unsigned general_number;
public:
	Object_Management() {
		srand(time(NULL));
		general_number = 0;
		ships = new Space_Ship * [100];
		rockets = new Rocket * [100];
		real_objects = new bool[max_objects_amount] { true };
		amount_deleted_obj = 0;
		match_table = new unsigned[max_objects_amount];
		counter = new unsigned[amount_types];
		for (unsigned i = 0; i < amount_types; ++i)
			counter[i] = 0;
		for (general_number; general_number < start_ship_number; ++general_number) {
			create_object(0, general_number);
			ships[general_number]->set_coord(set_random_starting_position(min_start_distance, max_start_distance));
			ships[general_number]->set_speed(set_random_ball_point(ships[general_number]->get_max_speed()));
			ships[general_number]->set_target();
			ships[general_number]->set_engine_power();

			//std::cout << arr_objects[general_number]->get_speed().x << "  "
				//<< arr_objects[general_number]->get_speed().y << "  " << arr_objects[general_number]->get_speed().z << std::endl;
		};

	};
	void print_objects()
	{
		for (unsigned k = 0; k < start_ship_number; ++k)
		{
			std::cout << "ship " << k << ':' << std::endl;
			(ships[k]->get_coord()).print("coord");
			(ships[k]->get_speed()).print("speed");
			(ships[k]->get_target()).print("target");
			std::cout << "target_distance:  " << (ships[k]->get_target() - ships[k]->get_coord()).get_module() << std::endl;
			(ships[k]->get_engine_power()).print("engine_power");
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
		return rockets[number % 100];
	}

	void create_object(unsigned type, unsigned number)
	{
		if (type >= amount_types) {
			std::cout << "Error: No this type of object" << std::endl;
			throw;
		}
		switch (type)
		{
		case 0: {ships[counter[type]] = new Space_Ship(); ships[counter[type]]->set_number(number); break; }
		case 1: {rockets[counter[type]] = new Rocket(); rockets[counter[type]]->set_number(number); break; }
			  //case 2: {arr_objects[counter[type] + 100 * type] = new Weapon(); break; }
			  //case 3: {arr_objects[counter[type] + 100 * type] = new Space_Ship(); break; }
		}
		match_table[number] = (counter[type] ++) + 100 * type;
	}

	void delete_object(unsigned number)
	{
		real_objects[number] = false;
	}

	void update_object(Type*** data, unsigned N)
	{
		for (unsigned current_type = 0; current_type < amount_types; ++current_type)
		{
			uint8_t* ptr = (uint8_t*)(&ships);
			using Type_ptr = Space_Ship**;
			switch (current_type)
			{
			case 1: {using Type_ptr = Rocket**; uint8_t* ptr = (uint8_t*)(&rockets); }
			}
			for (unsigned current_object = 0; current_object < counter[current_type]; ++current_object)
			{
				Type_ptr current_arr = (Type_ptr)(ptr);
				unsigned number = (current_arr)[current_object]->get_number();
				current_arr[current_object]->set_coord(data[0][number]);
				current_arr[current_object]->set_speed(data[1][number]);
				current_arr[current_object]->set_accel(data[2][number]);
			}
		}
	}

	void process_collisions(unsigned* arr1, unsigned* arr2, unsigned amount_collisions)
	{
		for (unsigned k = 0; k < amount_collisions; ++k)
		{
			if (arr1[k] / 100 == 0 && arr2[k] / 100 == 1)
			{
				if (find_ship(arr1[k])->get_damage((find_rocket(arr2[k]))->get_destructive_power()))
					delete_object(arr1[k]);
				real_objects[arr2[k]] = false;
			}
			if (arr1[k] / 100 == 1 && arr2[k] / 100 == 0)
			{
				real_objects[arr2[k] % 100] =
					find_ship(arr2[k])->get_damage((find_rocket(arr1[k]))->get_destructive_power());
				real_objects[arr1[k]] = false;
			}
		}
	}

	void process_events()
	{
		for (unsigned current_type = 0; current_type < amount_types; ++current_type)
		{
			for (unsigned current_object = 0; current_object < counter[current_type]; ++current_object)
			{
				switch (current_type) {
				case 0:
				{
					if (player_ship.get_coord().define_distance(ships[current_object]->get_coord()) <
						ships[current_object]->get_detected_distance())
					{
						ships[current_object]->set_target(player_ship.get_coord());
						if (player_ship.get_coord().define_distance(ships[current_object]->get_coord()) <
							ships[current_object]->get_fire_distance())
						{
							create_object(1, general_number++);
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

				}
				}
			}
		}
	}

	void send_changes(Type*** data, bool* exist_data)
	{
		for (unsigned current_type = 0; current_type < amount_types; ++current_type)
		{
			uint8_t* ptr = (uint8_t*)(&ships);
			using Type_ptr = Space_Ship**;
			switch (current_type)
			{
			case 1: {using Type_ptr = Rocket**; uint8_t* ptr = (uint8_t*)(&rockets); }
			}
			for (unsigned current_object = 0; current_object < counter[current_type]; ++current_object)
			{
				Type_ptr current_arr = (Type_ptr)(ptr);
				unsigned number = current_arr[current_object]->get_number();
				data[0][number] = current_arr[current_object]->get_list_coord();
				data[1][number] = current_arr[current_object]->get_list_speed();
				data[2][number] = current_arr[current_object]->get_list_accel();
			}
		}
	}


};


