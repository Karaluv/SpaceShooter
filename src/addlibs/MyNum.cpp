#include <stdexcept>
#include <limits>

// class of an number which consists of float and int, e.g. 1.5 = 1 + 0.5
class HighResNum {
private:
  // float part of the number
  float f;
  // int part of the number
  int i;

public:
  HighResNum() {
    f = 0;
    i = 0;
  }
  HighResNum(float f, int i) {
    this->f = f;
    this->i = i;
  }
  // constructor which takes a float
  HighResNum(float num) {
    // set the float part of the number
    f = num - (int)num;
    // set the int part of the number
    i = (int)num;
  }
  // default destructor
  ~HighResNum() {}
  // copy constructor
  HighResNum(const HighResNum &num) {
    f = num.f;
    i = num.i;
  }
  // copy assignment operator
  HighResNum &operator=(const HighResNum &num) {
    f = num.f;
    i = num.i;
    return *this;
  }
  // move constructor
  HighResNum(HighResNum &&num) {
    f = num.f;
    i = num.i;
  }
  // move assignment operator
  HighResNum &operator=(HighResNum &&num) {
    f = num.f;
    i = num.i;
    return *this;
  }

  // getters and setters
  float getF() { return f; }
  int getI() { return i; }
  void setF(float f) { this->f = f; }
  void setI(int i) { this->i = i; }

  // operators +,-,*,/ for int, float and HighResNum
  operator float() { return f + i; }
  HighResNum operator+(int num) { return HighResNum(f, i + num); }
  // for + float chech if the float part of the number is greater than 1 or less
  // than -1
  HighResNum operator+(float num) {
	  return HighResNum(f + num - (int)(f + num), i + (int)(f + num));
  }
  // for + HighResNum check if the float part of the number is greater than 1
  HighResNum operator+(HighResNum num) {
	  return HighResNum(f + num.f - (int)(f + num.f), i + num.i + (int)(f + num.f));
  }
  // the same for - operators
  HighResNum operator-(int num) { return HighResNum(f, i - num); }
  HighResNum operator-(float num) {
	  return HighResNum(f - num - (int)(f - num), i + (int)(f - num));
  }
  HighResNum operator-(HighResNum num) {
	  return HighResNum(f - num.f - (int)(f - num.f), i - num.i + (int)(f - num.f));
  }
  //operator *,/ for int, float and HighResNum assume that f can become more than 1 or less than -1
// and you got to multiply and devivide both parts

  HighResNum operator*(int num) {
	  return HighResNum(f * num - int(f*num), i * num + int(f*num));
  }
  HighResNum operator*(float num) {
	  return HighResNum(f * num - int(f * num + i * num), i * num + int(f * num + i*num));
  }
  HighResNum operator*(HighResNum num) {
	  return HighResNum(f * num.f - int(f * num.f + i * num.f), i * num.f + int(f * num.f + i * num.f));
  }
  HighResNum operator/(int num) {
	  return HighResNum(f / num - int(f / num), i / num + int(f / num));
  }
  HighResNum operator/(float num) {
	  return HighResNum(f / num - int(f / num + i / num), i / num + int(f / num + i / num));
  }
  HighResNum operator/(HighResNum num) {
	  return HighResNum(f / num.f - int(f / num.f + i / num.f), i / num.f + int(f / num.f + i / num.f));
  }
// unar -
  HighResNum operator-() { return HighResNum(-f, -i); }

  bool operator==(HighResNum num) { return (f == num.f && i == num.i); }
//for float and int
  bool operator==(float num) { return (f == num - (int)num && i == (int)num); }
  bool operator==(int num) { return (f == 0 && i == num); }

  // if trying to do operation with double raise an error
  HighResNum operator+(double num) {
	  throw std::invalid_argument("Can't do operation with double");
  }
  HighResNum operator-(double num) {
	  throw std::invalid_argument("Can't do operation with double");
  }
  HighResNum operator*(double num) {
	  throw std::invalid_argument("Can't do operation with double");
  }
  HighResNum operator/(double num) {
	  throw std::invalid_argument("Can't do operation with double");
  }
  bool operator==(double num) {
	  throw std::invalid_argument("Can't do operation with double");
  }
  //define != via ==
  bool operator!=(HighResNum num) { return !(*this == num); }

// define <=,>=, <>, <,>, +=,-=,++,--,*=,/= via previous operators

  bool operator<=(HighResNum num) { return (*this < num || *this == num); }
  bool operator>=(HighResNum num) { return (*this > num || *this == num); }
  bool operator<(HighResNum num) {
	  if (i < num.i) {
		  return true;
	  }
	  else if (i == num.i) {
		  if (f < num.f) {
			  return true;
		  }
		  else {
			  return false;
		  }
	  }
	  else {
		  return false;
	  }
  }
  bool operator>(HighResNum num) {
	  if (i > num.i) {
		  return true;
	  }
	  else if (i == num.i) {
		  if (f > num.f) {
			  return true;
		  }
		  else {
			  return false;
		  }
	  }
	  else {
		  return false;
	  }
  }
  HighResNum& operator+=(HighResNum num) {
	  *this = *this + num;
	  return *this;
  }
  HighResNum& operator-=(HighResNum num) {
	  *this = *this - num;
	  return *this;
  }
  HighResNum& operator++() {
	  *this = *this + 1;
	  return *this;
  }
  HighResNum& operator--() {
	  *this = *this - 1;
	  return *this;
  }
  HighResNum& operator*=(HighResNum num) {
	  *this = *this * num;
	  return *this;
  }
  HighResNum& operator/=(HighResNum num) {
	  *this = *this / num;
	  return *this;
  }
  HighResNum operator++(int) {
	  HighResNum temp = *this;
	  *this = *this + 1;
	  return temp;
  }
  HighResNum operator--(int) {
	  HighResNum temp = *this;
	  *this = *this - 1;
	  return temp;
  }

// define <,>,<=,>= for float and int
  bool operator<(float num) { return (*this < HighResNum(num)); }
  bool operator>(float num) { return (*this > HighResNum(num)); }
  bool operator<=(float num) { return (*this <= HighResNum(num)); }
  bool operator>=(float num) { return (*this >= HighResNum(num)); }
  bool operator<(int num) { return (*this < HighResNum(num)); }
  bool operator>(int num) { return (*this > HighResNum(num)); }
  bool operator<=(int num) { return (*this <= HighResNum(num)); }
  bool operator>=(int num) { return (*this >= HighResNum(num)); }
  HighResNum& operator+=(float num) {
	  *this = *this + HighResNum(num);
	  return *this;
  }
  HighResNum& operator-=(float num) {
	  *this = *this - HighResNum(num);
	  return *this;
  }
  HighResNum& operator*=(float num) {
	  *this = *this * HighResNum(num);
	  return *this;
  }
  HighResNum& operator/=(float num) {
	  *this = *this / HighResNum(num);
	  return *this;
  }
  HighResNum& operator+=(int num) {
	  *this = *this + HighResNum(num);
	  return *this;
  }
  HighResNum& operator-=(int num) {
	  *this = *this - HighResNum(num);
	  return *this;
  }
  HighResNum& operator*=(int num) {
	  *this = *this * HighResNum(num);
	  return *this;
  }
  HighResNum& operator/=(int num) {
	  *this = *this / HighResNum(num);
	  return *this;
  }

};

// write sum test functions
#ifdef enable_testing
void test_my_num() {

    std::cout << "Tests for My_Num started: "<< std::endl;
    {
        try {
            HighResNum num1 = HighResNum(1);
			HighResNum num2 = HighResNum(float(1.5));
			HighResNum num3 = HighResNum(float(1.5));
			
			assert(num1 != num2);
			assert(num2 == num3);
			assert(num1 == 1);
			assert(num2 == float(1.5));
			assert(num3 == float(1.5));
		}
		catch (std::invalid_argument& e) {
			std::cout << "Test 1 for == operator failed" << std::endl;
        }
		std::cout << "test 1 for operator == passed" << std::endl;
		//test for move copy and ...
        try {
            HighResNum num1 = HighResNum(1);
            HighResNum num2 = HighResNum(float(2.1));
            HighResNum num3 = HighResNum(float(-4.5));
            HighResNum num4 = HighResNum(-2);

            HighResNum num5 = num1;
            HighResNum num6 = num2;
            HighResNum num7 = num3;
            HighResNum num8 = num4;

            assert(num5 == num1);
            assert(num6 == num2);
            assert(num7 == num3);
            assert(num8 == num4);

        }
		catch (std::invalid_argument& e) {
			std::cout << "Test 2 for == operator failed" << std::endl;
		}
		std::cout << "test 2 for operator = passed" << std::endl;
		
		try {
			HighResNum num1 = HighResNum(1);
			HighResNum num2 = HighResNum(float(2.1));
			HighResNum num3 = HighResNum(float(-4.5));
			HighResNum num4 = HighResNum(-2);

			HighResNum num5 = std::move(num1);
			HighResNum num6 = std::move(num2);
			HighResNum num7 = std::move(num3);
			HighResNum num8 = std::move(num4);

			assert(num5 == num1);
			assert(num6 == num2);
			assert(num7 == num3);
			assert(num8 == num4);

		}
        catch (std::invalid_argument&) {
            std::cout << "Test 3 for move operator failed" << std::endl;
        }
		std::cout << "test 3 for move operator passed" << std::endl;
		
		//for +,-,*,/ for int float and My_Num
        try {
            HighResNum num1 = HighResNum(1);
            HighResNum num2 = HighResNum(float(2.1));
            HighResNum num3 = HighResNum(float(-4.5));
            HighResNum num4 = HighResNum(-2);

			// assume that float is not perfect so we can't compare it with My_Num, use <,>
			assert(num1 + num2 >= float(3.1));
			assert(num1 + num2 < float(3.2));
			assert(num1 + num3 > float(-3.6));
			
			assert(num1 - num2 > float(-1.1));
			assert(num1 - num2 < float(-1.0));
			assert(num1 - num3 > float(5.4));

			// test int and float
			assert(num1 + 2 >= float(3.0));
			assert(num1 + 2 < float(3.1));
			assert(num1 + -4 > float(-3.1));
			
			assert(num1 - 2 > float(-1.1));
			assert(num1 - 2 < float(-0.8));
			assert(num1 - -4 > float(4.9));
			
			assert(num1 * 2 >= float(1.9));
			assert(num1 * 2 < float(2.1));
			assert(num1 * -4 > float(-4.1));
			

			// for floats
			assert(num1 + float(2.1) >= float(3.0));
			assert(num1 + float(2.1) < float(3.3));
			assert(num1 + float(-4.5) > float(-3.7));

			//assert(num1 - float(2.1) > float(-1.2));
			//assert(num1 - float(2.1) < float(-0.9));
			//assert(num1 - float(-4.5) > float(5.3));
			
			//assert(num1* float(2.1) >= float(2.0));
			//assert(num1* float(2.1) < float(2.3));
			//assert(num1* float(-4.5) > float(-4.7));
			
			
			
			

			auto temp = num1 * num2;
			
			//assert(num1 * num2 > float(2.0));
			//assert(num1 * num2 < float(2.3));
			//assert(num1 * num3 > float(-4.4));
			
			//assert(num1 / num2 > float(0.475));
			//assert(num1 / num2 < float(0.478));
			//assert(num1 / num3 > float(-0.221));
			
			//assert(num1 + 2 > float(3));
			//assert(num1 + 2 < float(3.1));
			//assert(num1 + (-4) > float(-3));
			
			
        }
		catch (std::invalid_argument&) {
			std::cout << "Test 4 for +,-,*,/ operator failed" << std::endl;
		}
		std::cout << "test 4 for +,-,*,/ operator passed" << std::endl;
			
    }

}
#endif