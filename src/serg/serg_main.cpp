#include <iostream>

class Math_Point {
protected:
	double* coord;
	double* speed;
public:
	Math_Point(double x, double y, double z, double vx, double vy, double vz)
	{
		coord = new double[3];
		coord[0] = x;
		coord[1] = y;
		coord[2] = z;
		speed = new double[3];
		speed[0] = vx;
		speed[1] = vy;
		speed[2] = vz;
	};
	Math_Point(double x, double y, double z) : Math_Point(x, y, z, 0, 0, 0){};
	Math_Point() : coord(nullptr), speed(nullptr) {};
	void set_coord(double x, double y, double z) {
		if (coord == nullptr) {
			coord = new double[3];
			coord[0] = x;
			coord[1] = y;
			coord[2] = z;
		}
		else {
			std::cout << "ERROR: approach to change the coord of already existing object";
			return;
		}
	}
	void set_speed(double vx, double vy, double vz) {
		speed = new double[3];
		speed[0] = vx;
		speed[1] = vy;
		speed[2] = vz;
	}
};

class Massive_Point: public Math_Point {
protected:
	double* accel;
	double mass;
public:
	Massive_Point(double mass, double x, double y, double z, double vx, double vy, double vz) :
		Math_Point(x, y, z, vx, vy, vz), mass(mass), accel(nullptr) {};
	Massive_Point() : Math_Point(), mass(0), accel(nullptr) {};
	void move_object(double time, double* f) 
	{
		for (unsigned i = 0; i < 3; ++i) {
			accel[i] = f[i] / mass;
			coord[i] += speed[i] * time + accel[i] * time * time / 2;
			speed[i] += accel[i] * time;
		}
	}
};


class Space_Ship : public Massive_Point {
private:
	double max_speed;
	double max_accel;
	double size;
	int hp;
	unsigned* arsenal;
public:
	Space_Ship(double max_speed, double max_accel, unsigned hp, double size, unsigned* arsenal): Massive_Point(),
		max_speed(max_speed), max_accel(max_accel), hp(hp), size(size), arsenal(nullptr){}
	void destroy()
	{
		return; // дописать
	}
	void get_damage(double force) {
		hp -= force;
		if (hp <= 0) destroy();
	}
	void shout(unsigned weapon_type) {
		arsenal[weapon_type] --;

	}
};

class Weapon : Massive_Point {
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