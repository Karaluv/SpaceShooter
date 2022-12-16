#include <iostream>
#include <cassert>
#include <matrix.hpp>
#include <valarray>
#include <iterator>
#include <algorithm>
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
	

	
	
	void update_position(T time) {
		r = r + v * time;
	}
	
	void update_velocity(T time, directed_segment<T> force) {
		if (m == 0) {
			throw std::invalid_argument("Mass of body is 0");
		}
		v = v + (force/m) * time;
	}

	void update_w(T time, directed_segment<T> M) {

		if (tensor[0][0] == 0 || tensor[1][1] == 0 || tensor[2][2] == 0) {
			throw std::invalid_argument("Tensor of inertia is not defined");
		}
		directed_segment<T> w_;
		w_[0] = w[1] * w[2] * (tensor[1][1] - tensor[2][2]) / tensor[0][0] + M[0] / tensor[0][0];
		w_[1] = w[0] * w[2] * (tensor[2][2] - tensor[0][0]) / tensor[1][1] + M[1] / tensor[1][1];
		w_[2] = w[0] * w[1] * (tensor[0][0] - tensor[1][1]) / tensor[2][2] + M[2] / tensor[2][2];


		
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

	void collision(Body<T>& other) {
		if (this == &other) {
			throw std::invalid_argument("Collision with itself");
		}
		if (size == 0 || other.size == 0) {
			throw std::invalid_argument("Collision with zero size body");
		}
		if (m == 0 || other.m == 0) {
			throw std::invalid_argument("Collision with zero mass body");
		}
		
		directed_segment<T> v1 = (v * (m - other.m) + other.v* (2 * other.m)) / (m + other.m);
		
		directed_segment<T> v2 = (other.v * (other.m - m) + v*(2*m)) / (m + other.m);

		T collision_measure_1 = m*(v1 - v).length();
		T collision_measure_2 = other.m * (v2 - other.v).length();
		
		v = v1;
		other.v = v2;
		
		directed_segment<T> direction = other.r - r;
		direction = direction / direction.length();

		matrix<T, 3> A1(cos(angle[0]), -sin(angle[0]), 0, sin(angle[0]), cos(angle[0]), 0, 0, 0, 1);
		matrix<T, 3> A2(cos(angle[1]), 0, sin(angle[1]), 0, 1, 0, -sin(angle[1]), 0, cos(angle[1]));
		matrix<T, 3> A3(cos(angle[2]), -sin(angle[2]), 0, sin(angle[2]), cos(angle[2]), 0, 0, 0, 1);
		matrix<T, 3> A_1 = A1;
		matrix<T, 3> A_2 = A1 * A2 * A1.inverse();
		matrix<T, 3> A_3 = A1 * A2 * A3 * A2.inverse() * A1.inverse();
		matrix<T, 3> S = A3 * A2 * A1;

		directed_segment<T> w1 = S * w;
		directed_segment<T> w2 = S * other.w;
		directed_segment<T> w1_v = direction * (w1 * direction);
		directed_segment<T> w1_h = w1 - w1_v;
		directed_segment<T> w2_v = direction * (w2 * direction);
		directed_segment<T> w2_h = w2 - w2_v;
		
		directed_segment<T> delta_w_h = ( - w1_h) - w2_h;
		directed_segment<T> delta_w_v = ( - w1_v) + w2_v;
		
		directed_segment<T> M21 = (delta_w_h + delta_w_v*0.1) * size * collision_measure_1;
		directed_segment<T> M12 = -(delta_w_h + delta_w_v*0.1) * other.size * collision_measure_2;
		this->update_w(0.01, M21);
		other.update_w(0.01, M12);
	}
	
	T get_kinetic_energy() {
		return (m*(v * v))/2+(w * (tensor * w))/2;
	}

	Body operator+(const Body& other) {
		Body result;
		result.r = r + other.r;
		result.v = v + other.v;
		result.angle = angle + other.angle;
		result.w = w + other.w;
		return result;
	}

	Body operator*(const T& other) {
		Body result;
		result.r = r * other;
		result.v = v * other;
		result.angle = angle * other;
		result.w = w * other;
		return result;
	}


	Body operator/(const T& other) {
		Body result;
		result.r = r / other;
		result.v = v / other;
		result.angle = angle / other;
		result.w = w / other;
		return result;
	}



	
	
	
	
};
	
