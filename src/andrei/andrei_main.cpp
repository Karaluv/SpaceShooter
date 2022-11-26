#include <iostream>
#include <cassert>
#include <matrix.cpp>




// template T class body with fields m, tensor of inertia as matrix 3x3 , rx, ry, rz, w
template <typename T>
class Body {
public:
	T m;
	matrix<T,3> tensor;
	directed_segment<T> r;
	directed_segment<T> v;
	T w;
	T alpha;
	T size;

	Body(T m, matrix<T, 3> tensor, directed_segment<T> r, directed_segment<T> v, T w, T alpha, T size) : m(m), tensor(tensor), r(r), v(v), w(w), alpha(alpha), size(size) {}
	Body() : m(0), tensor(), r(), v(), w(0), alpha(0), size(0) {}

	//rule of five
	Body(const Body& other) : m(other.m), tensor(other.tensor), r(other.r), v(other.v), w(other.w), alpha(other.alpha), size(other.size) {}
	Body(Body&& other) : m(other.m), tensor(other.tensor), r(other.r), v(other.v), w(other.w), alpha(other.alpha), size(other.size) {}
	Body& operator=(const Body& other) {
		m = other.m;
		tensor = other.tensor;
		r = other.r;
		v = other.v;
		w = other.w;
		alpha = other.alpha;
		size = other.size;
		return *this;
	}
	Body& operator=(Body&& other) {
		m = other.m;
		tensor = other.tensor;
		r = other.r;
		v = other.v;
		w = other.w;
		alpha = other.alpha;
		size = other.size;
		return *this;
	}
	~Body() = default;


	
	// void which updates body position using velocity and time, updates alpha using w and time
	
	void update_position(T time) {
		r = r + v * time;
		alpha = alpha + w * time;
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

	//void which changes speed of rotation w, using tensor of inertia and other body

	
	
};
	
