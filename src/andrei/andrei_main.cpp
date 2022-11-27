#include <iostream>
#include <cassert>
#include <matrix.cpp>
#include <valarray>




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

	Body(T m, matrix<T, 3> tensor, directed_segment<T> r, directed_segment<T> v, directed_segment<T> angle, directed_segment<T> w, T size) :
		m(m), tensor(tensor), r(r), v(v), angle(angle), w(w), size(size) {};
	Body() : m(0), tensor(), r(), v(), angle(), w(), size(0) {};
	
	//rule of five
	
	Body(const Body& other) : m(other.m), tensor(other.tensor), r(other.r), v(other.v), angle(other.angle), w(other.w), size(other.size) {};
	Body(Body&& other) : m(other.m), tensor(other.tensor), r(other.r), v(other.v), angle(other.angle), w(other.w), size(other.size) {};
	Body& operator=(const Body& other) {
		m = other.m;
		tensor = other.tensor;
		r = other.r;
		v = other.v;
		angle = other.angle;
		w = other.w;
		size = other.size;
		return *this;
	}
	Body& operator=(Body&& other) {
		m = other.m;
		tensor = other.tensor;
		r = other.r;
		v = other.v;
		angle = other.angle;
		w = other.w;
		size = other.size;
		return *this;
	}
	~Body() = default;


	
	// void which updates body position using velocity and time, updates alpha using w and time
	
	void update_position(T time) {
		r = r + v * time;
	}

	void update_w(T time) {
		w[0] = w[0] + time * w[1] * w[2] * (tensor[1][1] - tensor[2][2]) / tensor[0][0];
		w[1] = w[1] + time * w[0] * w[2] * (tensor[2][2] - tensor[0][0]) / tensor[1][1];
		w[2] = w[2] + time * w[0] * w[1] * (tensor[0][0] - tensor[1][1]) / tensor[2][2];
	}	
	
	void update_angle(T time) {
		directed_segment<T> angle_;
		angle_[0] = (w[0] / cos(angle[2]) + w[1] / sin(angle[2])) / (sin(angle[1]) * (tan(angle[2]) + 1/tan(angle[2])));
		angle_[1] = (w[0] / sin(angle[2]) - w[1] / cos(angle[2])) / (tan(angle[2]) + 1/tan(angle[2]));
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

	//calculation of kinetic moment using tensor of inertia and angular velocity
	
	
	
};
	
