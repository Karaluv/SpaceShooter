#include <iostream>
#include <cassert>




template <typename T>
class directed_segment {
public:
	T x;
	T y;
	T z;

	directed_segment() {
		x = 0;
		y = 0;
		z = 0;
	}

	directed_segment(T x, T y, T z) : x(x), y(y), z(z) {}

	//rule of 5 operators

	directed_segment(const directed_segment& other) {
		x = other.x;
		y = other.y;
		z = other.z;
	}

	directed_segment& operator=(const directed_segment& other) {
		x = other.x;
		y = other.y;
		z = other.z;
		return *this;
	}

	directed_segment(directed_segment&& other){
		x = other.x;
		y = other.y;
		z = other.z;
	}

	directed_segment& operator=(directed_segment&& other) {
		x = other.x;
		y = other.y;
		z = other.z;
		return *this;
	}

	~directed_segment() {
		x = 0;
		y = 0;
		z = 0;
	}

	//operator+

	directed_segment operator+(const directed_segment& other) {
		directed_segment result;
		result.x = x + other.x;
		result.y = y + other.y;
		result.z = z + other.z;
		return result;
	}

	//operator-

	directed_segment operator-(const directed_segment& other) {
		directed_segment result;
		result.x = x - other.x;
		result.y = y - other.y;
		result.z = z - other.z;
		return result;
	}

	//operator*, scalar multiplication

	directed_segment operator*(const T& scalar) {
		directed_segment result;
		result.x = x * scalar;
		result.y = y * scalar;
		result.z = z * scalar;
		return result;
	}

	//operator /
	
	directed_segment operator/(const T& scalar) {
		directed_segment result;
		result.x = x / scalar;
		result.y = y / scalar;
		result.z = z / scalar;
		return result;
	}

	//operator*, dot product

	T operator*(const directed_segment& other) {
		T result;
		result = x * other.x + y * other.y + z * other.z;
		return result;
	}

	//operator^, cross product

	directed_segment operator^(const directed_segment& other) {
		directed_segment result;
		result.x = y * other.z - z * other.y;
		result.y = z * other.x - x * other.z;
		result.z = x * other.y - y * other.x;
		return result;
	}

	//operator==

	bool operator==(const directed_segment& other) {
		return x == other.x && y == other.y && z == other.z;
	}

	//operator!=

	bool operator!=(const directed_segment& other) {
		return x != other.x || y != other.y || z != other.z;
	}

	//operator+=

	directed_segment& operator+=(const directed_segment& other) {
		x += other.x;
		y += other.y;
		z += other.z;
		return *this;
	}

	//operator-=

	directed_segment& operator-=(const directed_segment& other) {
		x -= other.x;
		y -= other.y;
		z -= other.z;
		return *this;
	}

	//operator*=
	
	directed_segment& operator*=(const T& scalar) {
		x *= scalar;
		y *= scalar;
		z *= scalar;
		return *this;
	}

		
	//operator /=

	directed_segment& operator/=(const T& scalar) {
		x /= scalar;
		y /= scalar;
		z /= scalar;
		return *this;
	}


	//operator^=

	directed_segment& operator^=(const directed_segment& other) {
		T x1 = x;
		T y1 = y;
		T z1 = z;
		x = y1 * other.z - z1 * other.y;
		y = z1 * other.x - x1 * other.z;
		z = x1 * other.y - y1 * other.x;
		return *this;
	}

	//operator<<

	friend std::ostream& operator<<(std::ostream& os, const directed_segment& other) {
		os << "(" << other.x << ", " << other.y << ", " << other.z << ")";
		return os;
	}

	//operator>>


	friend std::istream& operator>>(std::istream& is, directed_segment& other) {
		is >> other.x >> other.y >> other.z;
		return is;
	}

	//operator[]

	T& operator[](int index) {
		assert(index >= 0 && index < 3);
		if (index == 0) {
			return x;
		}
		else if (index == 1) {
			return y;
		}
		else {
			return z;
		}
	}


	//operator[] const
	
	const T& operator[](int index) const {
		assert(index >= 0 && index < 3);
		if (index == 0) {
			return x;
		}
		else if (index == 1) {
			return y;
		}
		else {
			return z;
		}
	}

};




// class for matrix with template type T and template size N
template <typename T, size_t N>
class matrix {
public:
	T data[N][N];

	matrix() {
		for (size_t i = 0; i < N; i++) {
			for (size_t j = 0; j < N; j++) {
				data[i][j] = 0;
			}
		}
	}

	matrix(T data[N][N]) {
		for (size_t i = 0; i < N; i++) {
			for (size_t j = 0; j < N; j++) {
				this->data[i][j] = data[i][j];
			}
		}
	}

	//constructor for N^2 arhuments
	template <typename... Args>
	matrix(Args... args) {
		assert(sizeof...(args) == N * N);
		T arr[N * N] = { args... };
		for (size_t i = 0; i < N; i++) {
			for (size_t j = 0; j < N; j++) {
				data[i][j] = arr[i * N + j];
			}
		}
	}

	//rule of 5 operators

	matrix(const matrix& other) {
		for (size_t i = 0; i < N; i++) {
			for (size_t j = 0; j < N; j++) {
				data[i][j] = other.data[i][j];
			}
		}
	}

	matrix& operator=(const matrix& other) {
		for (size_t i = 0; i < N; i++) {
			for (size_t j = 0; j < N; j++) {
				data[i][j] = other.data[i][j];
			}
		}
		return *this;
	}

	matrix(matrix&& other) {
		for (size_t i = 0; i < N; i++) {
			for (size_t j = 0; j < N; j++) {
				data[i][j] = other.data[i][j];
			}
		}
	}

	matrix& operator=(matrix&& other) {
		for (size_t i = 0; i < N; i++) {
			for (size_t j = 0; j < N; j++) {
				data[i][j] = other.data[i][j];
			}
		}
		return *this;
	}

	~matrix() {
		for (size_t i = 0; i < N; i++) {
			for (size_t j = 0; j < N; j++) {
				data[i][j] = 0;
			}
		}
	}

	//operator+

	matrix operator+(const matrix& other) {
		matrix result;
		for (size_t i = 0; i < N; i++) {
			for (size_t j = 0; j < N; j++) {
				result.data[i][j] = data[i][j] + other.data[i][j];
			}
		}
		return result;
	}

	//operator-

	matrix operator-(const matrix& other) {
		matrix result;
		for (size_t i = 0; i < N; i++) {
			for (size_t j = 0; j < N; j++) {
				result.data[i][j] = data[i][j] - other.data[i][j];
			}
		}
		return result;
	}

	//operator*, matrix multiplication

	matrix operator*(const matrix& other) {
		matrix result;
		for (size_t i = 0; i < N; i++) {
			for (size_t j = 0; j < N; j++) {
				for (size_t k = 0; k < N; k++) {
					result.data[i][j] += data[i][k] * other.data[k][j];
				}
			}
		}
		return result;
	}

	//operator*, scalar multiplication

	matrix operator*(const T& scalar) {
		matrix result;
		for (size_t i = 0; i < N; i++) {
			for (size_t j = 0; j < N; j++) {
				result.data[i][j] = data[i][j] * scalar;
			}
		}
		return result;
	}

	//operator*, multiplication of matrix and directed segment
	
	directed_segment<T> operator*(const directed_segment<T>& other) {
		directed_segment<T> result;
		for (size_t i = 0; i < N; i++) {
			for (size_t j = 0; j < N; j++) {
				result[i] += data[i][j] * other[j];
			}
		}
		return result;
	}

	//operator==

	bool operator==(const matrix& other) {
		for (size_t i = 0; i < N; i++) {
			for (size_t j = 0; j < N; j++) {
				if (data[i][j] != other.data[i][j]) {
					return false;
				}
			}
		}
		return true;
	}

	//operator!=

	bool operator!=(const matrix& other) {
		for (size_t i = 0; i < N; i++) {
			for (size_t j = 0; j < N; j++) {
				if (data[i][j] != other.data[i][j]) {
					return true;
				}
			}
		}
		return false;
	}

	//operator+=

	matrix& operator+=(const matrix& other) {
		for (size_t i = 0; i < N; i++) {
			for (size_t j = 0; j < N; j++) {
				data[i][j] += other.data[i][j];
			}
		}
		return *this;
	}

	//operator-=

	matrix& operator-=(const matrix& other) {
		for (size_t i = 0; i < N; i++) {
			for (size_t j = 0; j < N; j++) {
				data[i][j] -= other.data[i][j];
			}
		}
		return *this;
	}

	//operator*=, matrix multiplication

	matrix& operator*=(const matrix& other) {
		matrix result;
		for (size_t i = 0; i < N; i++) {
			for (size_t j = 0; j < N; j++) {
				for (size_t k = 0; k < N; k++) {
					result.data[i][j] += data[i][k] * other.data[k][j];
				}
			}
		}
		*this = result;
		return *this;
	}

	//operator*=, scalar multiplication

	matrix& operator*=(const T& scalar) {
		for (size_t i = 0; i < N; i++) {
			for (size_t j = 0; j < N; j++) {
				data[i][j] *= scalar;
			}
		}
		return *this;
	}

	//operator[]

	T* operator[](size_t index) {
		return data[index];
	}

	//operator() const

	const T& operator()(size_t i, size_t j) const {
		return data[i][j];
	}

	//operator()

	T& operator()(size_t i, size_t j) {
		return data[i][j];
	}

	//transpose

	matrix transpose() {
		matrix result;
		for (size_t i = 0; i < N; i++) {
			for (size_t j = 0; j < N; j++) {
				result.data[i][j] = data[j][i];
			}
		}
		return result;
	}

	//determinant

	T determinant_func(size_t n) {
		if (n == 1) {
			return data[0][0];
		}
		else if (n == 2) {
			return data[0][0] * data[1][1] - data[0][1] * data[1][0];
		}
		else {
			T result = 0;
			for (size_t i = 0; i < n; i++) {
				matrix<T, N> temp;
				for (size_t j = 0; j < n - 1; j++) {
					for (size_t k = 0; k < n - 1; k++) {
						if (k < i) {
							temp.data[j][k] = data[j + 1][k];
						}
						else {
							temp.data[j][k] = data[j + 1][k + 1];
						}
					}
				}
				result += data[0][i] * temp.determinant_func(n - 1) * (i % 2 == 0 ? 1 : -1);
			}
			return result;
		}
	}

	T determinant() {
		return determinant_func(N);
	}


	//inverse

	matrix inverse() {
		matrix result;
		T det = determinant();
		if (det == 0) {
			throw std::runtime_error("Matrix is not invertible");
		}
		for (size_t i = 0; i < N; i++) {
			for (size_t j = 0; j < N; j++) {
				matrix<T, N> temp;
				for (size_t k = 0; k < N - 1; k++) {
					for (size_t l = 0; l < N - 1; l++) {
						if (k < i) {
							if (l < j) {
								temp.data[k][l] = data[k][l];
							}
							else {
								temp.data[k][l] = data[k][l + 1];
							}
						}
						else {
							if (l < j) {
								temp.data[k][l] = data[k + 1][l];
							}
							else {
								temp.data[k][l] = data[k + 1][l + 1];
							}
						}
					}
				}
				result.data[j][i] = temp.determinant_func(N - 1) * (i % 2 == j % 2 ? 1 : -1) / det;
			}
		}
		return result;
	}

	//print

	void print() {
		for (size_t i = 0; i < N; i++) {
			for (size_t j = 0; j < N; j++) {
				std::cout << data[i][j] << " ";
			}
			std::cout << std::endl;
		}
	}

	// power operator 

	matrix operator^(const int& power) {
		matrix result;
		for (size_t i = 0; i < N; i++) {
			result.data[i][i] = 1;
		}
		matrix temp = *this;
		int p = power;
		while (p > 0) {
			if (p % 2 == 1) {
				result *= temp;
			}
			temp *= temp;
			p /= 2;
		}
		return result;
	}


};




//tests
void test_directed_segment()
{
	//for each operator, a test

	directed_segment<int> a(1, 2, 3);
	directed_segment<int> b(4, 5, 6);
	directed_segment<int> c(7, 8, 9);
	//tests for copy and assigment
	directed_segment<int> d(a);
	directed_segment<int> e;
	e = a;
	assert(a == d);
	assert(a == a);
	assert(a != b);
	assert(a + b == directed_segment<int>(5, 7, 9));
	assert(a - b == directed_segment<int>(-3, -3, -3));
	assert(a * 2 == directed_segment<int>(2, 4, 6));
	assert(a * b == 32);
	assert((a ^ b) == directed_segment<int>(-3, 6, -3));
	assert((a += b) == directed_segment<int>(5, 7, 9));
	assert((a -= b) == directed_segment<int>(1, 2, 3));

	//memory leak test for move constructors and copy constructors
	for (int i = 0; i < 1000000; i++) {
		directed_segment<int> d = a;
		directed_segment<int> e = std::move(d);
	}

	std::cout << "Test passed" << std::endl;

}

void test_matrix()
{
	//test for matrix

	matrix<int, 3> a;
	a(0, 0) = 1;
	a(0, 1) = 2;
	a(0, 2) = 3;
	a(1, 0) = 4;

	matrix<int, 3> b;
	b(0, 0) = 1;
	b(0, 1) = 2;
	b(0, 2) = 3;
	b(1, 0) = 5;

	matrix<int, 3> c;
	c(0, 0) = 1;
	c(0, 1) = 2;
	c(0, 2) = 3;
	c(1, 0) = 4;

	matrix<int, 3> d;
	d(0, 0) = 2;
	d(0, 1) = 5;
	d(0, 2) = 7;
	d(1, 0) = 6;
	d(1, 1) = 3;
	d(1, 2) = 4;
	d(2, 0) = 5;
	d(2, 1) = -2;
	d(2, 2) = -3;

	matrix<int, 3> e;
	e(0, 0) = 34;
	e(0, 1) = 2;
	e(0, 2) = 3;
	e(1, 0) = 4;
	e(1, 1) = 5;
	e(1, 2) = 6;
	e(2, 0) = 7;
	e(2, 1) = 56;
	e(2, 2) = 9;

	assert(a == a);
	assert(a != b);
	assert((a + b) == (matrix<int, 3>(2, 4, 6, 9, 0, 0, 0, 0, 0)));
	assert((a - b) == (matrix<int, 3>(0, 0, 0, -1, 0, 0, 0, 0, 0)));
	assert((a * 2) == (matrix<int, 3>(2, 4, 6, 8, 0, 0, 0, 0, 0)));
	assert((a * b) == (matrix<int, 3>(11, 2, 3, 4, 8, 12, 0, 0, 0)));
	assert((a ^ 2) == (matrix<int, 3>(9, 2, 3, 4, 8, 12, 0, 0, 0)));
	assert((a += b) == (matrix<int, 3>(2, 4, 6, 9, 0, 0, 0, 0, 0)));
	assert((a -= b) == (matrix<int, 3>(1, 2, 3, 4, 0, 0, 0, 0, 0)));
	assert(a.determinant() == 0);
	assert(d.determinant() == -1);
	assert(e.determinant() == -9315);
	assert(d.inverse() * d == (matrix<int, 3>(1, 0, 0, 0, 1, 0, 0, 0, 1)));
	assert(d.inverse() == (matrix<int, 3>(1, -1, 1, -38, 41, -34, 27, -29, 24)));

	try {

		a.inverse();
	}
	catch (std::runtime_error& e)
	{
		std::cout << e.what() << std::endl;
	}

	//memory leak test for move constructors and copy constructors

	for (int i = 0; i < 1000000; i++) {
		matrix<int, 3> f = a;
		matrix<int, 3> g = std::move(f);
	}

	std::cout << "Test passed" << std::endl;

}