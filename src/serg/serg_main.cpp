#include <iostream>
#include <optional>

class Math_Point;
class Massive_Point;
class Space_Ship;
class Weapon;
class Object_Management;


class Math_Point {
protected:
	unsigned number;
	double* coord;
	double* speed;
	double* accel;

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
	Math_Point() : number(0), coord(nullptr), speed(nullptr) {};
	void set_coord(double* arr) {
		coord = new double[3];
		for (unsigned i = 0; i < 3; ++i)
		{
			coord[i] = arr[i];
		}
	}
	void set_speed(double* v) {
		speed = new double[3];
		for (unsigned i = 0; i < 3; ++i)
		{
			speed[i] = v[i];
		}
	}

	void set_accel(double* a) {
		accel = new double[3];
		for (unsigned i = 0; i < 3; ++i)
		{
			accel[i] = a[i];
		}
	}

	void set_number(unsigned n)
	{
		number = n;
	}
	unsigned get_number()
	{
		return number;
	}

	double* get_coord()
	{
		return coord;
	}

	double* get_speed()
	{
		return coord;
	}

	double* get_accel()
	{
		return accel;
	}
};

class Massive_Point: public Math_Point {
protected:
	double mass;
public:
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
};


class Space_Ship : public Massive_Point {
private:
	double max_speed;
	double max_accel;
	double size;
	int hp;
	unsigned* arsenal;
public:
	Space_Ship(): max_speed(0), max_accel(0), size(0), hp(0), arsenal(nullptr) {};

	Space_Ship(double max_speed, double max_accel, unsigned hp, double size, unsigned* arsenal): Massive_Point(),
		max_speed(max_speed), max_accel(max_accel), hp(hp), size(size), arsenal(nullptr){}
	void destroy()
	{
		return; // дописать
	}
	void get_damage(double force) {
		hp -= (static_cast<int>(force));
		if (hp <= 0) destroy();
	}
	void shout(unsigned weapon_type) {
		arsenal[weapon_type] --;

	}
};

class Weapon : public Massive_Point {
protected:
	double start_speed;
	unsigned destructive_power;
public:
	Weapon(double start_speed, unsigned destructive_power) : Massive_Point(), start_speed(start_speed), 
		destructive_power(destructive_power) {};
	Weapon(): Massive_Point(), start_speed(0), destructive_power(0) {};
	void set_parametres(double start_speed, unsigned destructive_power) {
		this->start_speed = start_speed;
		this->destructive_power = destructive_power;
	}
	void cause_damage(Space_Ship* ship, double force) {
		ship->get_damage(force * destructive_power);
	}
};

class Object_Management {
private:
	unsigned const amount_types = 4;
	Math_Point** arr_objects;
	unsigned* match_table;
	unsigned* counter;
public:
	Object_Management() {
		arr_objects = new Math_Point * [1000];
		match_table = new unsigned[100000];
		counter = new unsigned[amount_types];
		for (unsigned i = 0; i < amount_types; ++i)
			counter[i] = 0;
	};
	void create_object(unsigned type, unsigned number)
	{
		if (type >= amount_types) {
			std::cout << "Error: No this type of object" << std::endl;
			throw;
		}
		switch (type)
		{
		case 0: {arr_objects[counter[type] + 100 * type] = new Math_Point();  break; }
		case 1: {arr_objects[counter[type] + 100 * type] = new Massive_Point(); break; }
		case 2: {arr_objects[counter[type] + 100 * type] = new Weapon(); break; }
		case 3: {arr_objects[counter[type] + 100 * type] = new Space_Ship(); break; }
		}
		arr_objects[counter[type] + 100 * type]->set_number(number);
		match_table[number] = (counter[type] ++) + 100 * type;
	}

	void update_object(double*** data, unsigned N)
	{
		for (unsigned current_type = 0; current_type < amount_types; ++current_type)
		{
			for (unsigned current_object = 0; current_object < counter[current_type]; ++current_object)
			{
				unsigned number = arr_objects[current_object + 100 * current_type]->get_number();
				arr_objects[current_object + 100 * current_type]->set_coord(data[0][number]);
				arr_objects[current_object + 100 * current_type]->set_speed(data[1][number]);
				arr_objects[current_object + 100 * current_type]->set_accel(data[2][number]);
			}
		}
	}
	void process_events()
	{
		for (unsigned current_type = 0; current_type < amount_types; ++current_type)
		{
			for (unsigned current_object = 0; current_object < counter[current_type]; ++current_object)
			{
				double* speed = arr_objects[current_object + 100 * current_type]->get_speed();
				double a[3] = { speed[0] / 10 , speed[0] / 10 , speed[0] / 10 };
				arr_objects[current_object + 100 * current_type]->set_accel(a);
			}
		}
	}

	void send_changes(double*** data)
	{
		for (unsigned current_type = 0; current_type < amount_types; ++current_type)
		{
			for (unsigned current_object = 0; current_object < counter[current_type]; ++current_object)
			{
				unsigned number = arr_objects[current_object + 100 * current_type]->get_number();
				//data[0][number]
				//arr_objects[current_object + 100 * current_type]->set_coord(data[0][number]);
				//arr_objects[current_object + 100 * current_type]->set_speed(data[1][number]);
				//arr_objects[current_object + 100 * current_type]->set_accel(data[2][number]);
			}
		}
	}


};
