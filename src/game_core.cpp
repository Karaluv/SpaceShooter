#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#include <iostream>
#include <thread>
#include <chrono>

#include <render.cpp>
#include <serg_main.cpp>
#include <andrei_main.cpp>




// varient of dictionery where all keys are values and there are 4 int values in total
// e.g. pairs: dict = {(1,12,434,52),(424,2748,23,9)} and if I call dict[3][52] it will return (1,12,434,52)

class Dict4X
{
private:
	//vector of unsigned pointers 
	std::vector<unsigned*> dict;
	
public:
	//constructor
	Dict4X()
	{
		//create a new vector of unsigned pointers
		dict = std::vector<unsigned*>();
	}

	//destructor
	~Dict4X()
	{
		//delete all unsigned pointers in the vector
		for (unsigned i = 0; i < dict.size(); i++)
		{
			delete dict[i];
		}
	}
	//add new key to the dictionary
	void add(unsigned a, unsigned b, unsigned c, unsigned d)
	{
		//create a new unsigned pointer
		unsigned* newKey = new unsigned[4];
		//set the values of the new key
		newKey[0] = a;
		newKey[1] = b;
		newKey[2] = c;
		newKey[3] = d;
		//add the new key to the vector
		dict.push_back(newKey);
	}
	// add using pointer
	void add(unsigned* key)
	{
		//create a new unsigned pointer
		unsigned* newKey = new unsigned[4];
		//set the values of the new key
		newKey[0] = key[0];
		newKey[1] = key[1];
		newKey[2] = key[2];
		newKey[3] = key[3];
		//add the new key to the vector
		dict.push_back(newKey);
	}
	// remove key using index in vector
	void remove(unsigned index)
	{
		//delete the unsigned pointer at the index
		delete dict[index];
		//remove the pointer from the vector
		dict.erase(dict.begin() + index);
	}
	// search function which takes two arguments: which key to search for and its position in sub-arrays
	unsigned search(unsigned key, unsigned pos)
	{
		//loop through all keys in the vector
		for (unsigned i = 0; i < dict.size(); i++)
		{
			//if the key is found return the index
			if (dict[i][pos] == key)
			{
				return i;
			}
		}
		//if the key is not found return -1
		return -1;
	}
	// add operator []
	unsigned* operator[](unsigned index)
	{
		//return the unsigned pointer at the index
		return dict[index];
	}
	// return the size of the vector
	unsigned size()
	{
		return dict.size();
	}
	
};
	



int main()
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	
	//run tests from andrei
	test_directed_segment();
	test_matrix();

	
	srs::start_render();
	{
		// create monkey and cube and several lights in different positions and angels
		srs::create_object("monkey", 0, 0, -1, 0, 0, 0, 1);
		srs::create_object("cube", 4, 0, -1, 0, 0, 0, 1);

		// now I need to create a light
		srs::create_light(8, 3, -2, 1, 1, 1, 50);
		srs::create_light(-8, -3, -2, 1, 1, 1, 50);

		// write some code 144 times per seconds to update camera position, objects position, lights position and color
		for (int i = 0; i < 10000; i++)
		{
			// rotate monkey by sin i around y axis
			srs::update_object(0, 0, 0, -1, 0, sin(float(i) / 100), 0, 1);
			// move cube by sin i around y axis
			srs::update_object(1, 4 + sin(float(i) / 100), 0, -1, 0, 0, 0, 1);

			// change light color by sin i
			srs::update_light(0, 8, 3, -2, sin(float(i) / 100), 0, 0, 100);
			// srs::update_camera(0.0, 0.0, 0.0, 0.0, 0.0, 0.0);
			//  cube move by 0.0001 in x direction
			// srs::update_object(0, 0, 0, -1, 0.0, 0.0, 0.0, 1.0);

			std::this_thread::sleep_for(std::chrono::milliseconds(1));
		}
	}
	srs::stop_render();

	return 0;
}
