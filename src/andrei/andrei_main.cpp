#include <iostream>
#include <cassert>
#include <matrix.hpp>
#include <valarray>

// super accurate sin, tan, cos functions
#include <cmath>




template<typename State>
class GenericRK {
private:
	State k_1_res;
	State k_2_res;
	State k_3_res;
	State k_4_res;
	double dt = 0.01;

	void k_1(State& state) {
		k_1_res = state.f(state) * dt;
	}

	void k_2(State& state) {
		State mezo_state;
		mezo_state = state + k_1_res / 2.0;
		k_2_res = mezo_state.f(mezo_state) * dt;
	}

	void k_3(State& state) {
		State mezo_state;
		mezo_state = state + k_2_res / 2.0;
		k_3_res = mezo_state.f(mezo_state) * dt;
	}

	void k_4(State& state) {
		State mezo_state;
		mezo_state = state + k_3_res;
		k_4_res = mezo_state.f(mezo_state) * dt;
	}
public:
	State step(State& state) {
		k_1(state);
		k_2(state);
		k_3(state);
		k_4(state);
		State new_state;
		new_state = state + (k_1_res + k_2_res * 2 + k_3_res * 2 + k_4_res) / 6.0;
		return new_state;
	}
};







// template T class body with fields m, tensor of inertia as matrix 3x3 , rx, ry, rz, w
template <typename T>
class Body {
public:
	T m;
	matrix<T,3> tensor;
	directed_segment<T> r;
	directed_segment<T> v;
	directed_segment<T> angle;
	directed_segment<T> w;
	T size;
	T kinetic_energy_of_rotation;

	Body(T m, matrix<T, 3> tensor, directed_segment<T> r, directed_segment<T> v, directed_segment<T> angle, directed_segment<T> w, T size) :
		m(m), tensor(tensor), r(r), v(v), angle(angle), w(w), size(size) {
		kinetic_energy_of_rotation = (w * (tensor * w)) / 2;
	};
	Body() : m(0), tensor(), r(), v(), angle(), w(), size(0) {
		kinetic_energy_of_rotation = 0;
	};
	
	//rule of five
	Body(const Body& other) : m(other.m), tensor(other.tensor), r(other.r), v(other.v), angle(other.angle), w(other.w), size(other.size), kinetic_energy_of_rotation(other.kinetic_energy_of_rotation) {};
	Body(Body&& other) : m(other.m), tensor(other.tensor), r(other.r), v(other.v), angle(other.angle), w(other.w), size(other.size), kinetic_energy_of_rotation(other.kinetic_energy_of_rotation) {};
	Body& operator=(const Body& other) {
		m = other.m;
		tensor = other.tensor;
		r = other.r;
		v = other.v;
		angle = other.angle;
		w = other.w;
		size = other.size;
		kinetic_energy_of_rotation = other.kinetic_energy_of_rotation;
		return *this;
	};
	Body& operator=(Body&& other) {
		m = other.m;
		tensor = other.tensor;
		r = other.r;
		v = other.v;
		angle = other.angle;
		w = other.w;
		size = other.size;
		kinetic_energy_of_rotation = other.kinetic_energy_of_rotation;
		return *this;
	};
	~Body() {};
	


	
	// void which updates body position using velocity and time, updates alpha using w and time




	Body f(Body& state) {
		Body result;
		result.w[0] = (state.tensor[1][1] - state.tensor[2][2]) / state.tensor[0][0] * state.w[1] * state.w[2];
		result.w[1] = (state.tensor[2][2] - state.tensor[0][0]) / state.tensor[1][1] * state.w[2] * state.w[0];
		result.w[2] = state.tensor[2][2] * (state.tensor[0][0] - state.tensor[1][1]) / state.tensor[2][2] * state.w[0] * state.w[1];
		result.angle[0] = (state.w[0] * sin(state.angle[2]) + state.w[1] * cos(state.angle[2])) / (cos(state.angle[1]));
		result.angle[1] = state.w[0] * cos(state.angle[2]) - state.w[1] * sin(state.angle[2]);
		result.angle[2] = state.w[2] - (state.w[0] * sin(state.angle[2]) + state.w[1] * cos(state.angle[2])) / tan(state.angle[1]);
		return result;
	}
	/*
	void update_rotation() {
		GenericRK<Body> RK;
		Body bd = RK.step(*this);
		this = bd;
	}
	*/

	
	
	void update_position(T time) {
		r = r + v * time;
	}

	void update_w(T time) {
		directed_segment<T> w_;
		w_[0] = w[1] * w[2] * (tensor[1][1] - tensor[2][2]) / tensor[0][0];
		w_[1] = w[0] * w[2] * (tensor[2][2] - tensor[0][0]) / tensor[1][1];
		w_[2] = w[0] * w[1] * (tensor[0][0] - tensor[1][1]) / tensor[2][2];


		
		w[0] = w[0] + time * w_[0];
		w[1] = w[1] + time * w_[1];
		w[2] = w[2] + time * w_[2];

		T kin = (w * (tensor * w)) / 2;
		T k = sqrt(kinetic_energy_of_rotation/kin);
		w = w * k;
	}	
	
	void update_angle(T time) {
		directed_segment<T> angle_;

		angle_[0] = (w[0] / cos(angle[2]) + w[1] / sin(angle[2])) * sin(2 * angle[2]) / (2 * sin(angle[1]));
		angle_[1] = (w[0] / sin(angle[2]) - w[1] / cos(angle[2])) * sin(2*angle[2]) / (2);
		angle_[2] = w[2] - angle_[0]*cos(angle[1]);
		angle = angle + angle_ * time;
	}

	// void collision with other body using vector of velocity
	void collision(Body<T>& other, directed_segment<T> v) {
		// calculate new velocity for this body
		directed_segment<T> v1 = (v * (m - other.m) + 2 * other.m * other.v) / (m + other.m);
		// calculate new velocity for other body
		directed_segment<T> v2 = (v * (other.m - m) + 2 * m * v) / (m + other.m);
		// update velocity for this body
		v = v1;
		// update velocity for other body
		other.v = v2;
	}

	//get kinetic energy including rotation using tensor
	T get_kinetic_energy() {
		return (m*(v * v))/2+(w * (tensor * w))/2;
	}







	//operator + for body
	Body operator+(const Body& other) {
		Body result;
		result.r = r + other.r;
		result.v = v + other.v;
		result.angle = angle + other.angle;
		result.w = w + other.w;
		return result;
	}
	//operator * for body
	Body operator*(const T& other) {
		Body result;
		result.r = r * other;
		result.v = v * other;
		result.angle = angle * other;
		result.w = w * other;
		return result;
	}

	//operator / for body
	Body operator/(const T& other) {
		Body result;
		result.r = r / other;
		result.v = v / other;
		result.angle = angle / other;
		result.w = w / other;
		return result;
	}



	
	
	
	
};
	
