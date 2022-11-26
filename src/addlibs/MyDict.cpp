
#include <vector>

#include <cassert>


class Dict4X {
private:
  // vector of unsigned pointers
  std::vector<unsigned *> dict;

public:
  // constructor
  Dict4X() {
    // create a new vector of unsigned pointers
    dict = std::vector<unsigned *>();
  }

  // destructor
  ~Dict4X() {
    // delete all unsigned pointers in the vector
    for (unsigned i = 0; i < dict.size(); i++) {
      delete dict[i];
    }
  }
  // add new key to the dictionary
  void add(unsigned a, unsigned b, unsigned c, unsigned d) {
    // create a new unsigned pointer
    unsigned *newKey = new unsigned[4];
    // set the values of the new key
    newKey[0] = a;
    newKey[1] = b;
    newKey[2] = c;
    newKey[3] = d;
    // add the new key to the vector
    dict.push_back(newKey);
  }
  // add using pointer
  void add(unsigned *key) {
    // create a new unsigned pointer
    unsigned *newKey = new unsigned[4];
    // set the values of the new key
    newKey[0] = key[0];
    newKey[1] = key[1];
    newKey[2] = key[2];
    newKey[3] = key[3];
    // add the new key to the vector
    dict.push_back(newKey);
  }
  // remove key using index in vector
  void remove(unsigned index) {
    // delete the unsigned pointer at the index
    delete dict[index];
    // remove the pointer from the vector
    dict.erase(dict.begin() + index);
  }
  // search function which takes two arguments: which key to search for and its
  // position in sub-arrays
  unsigned search(unsigned key, unsigned pos) {
    // loop through all keys in the vector
    for (unsigned i = 0; i < dict.size(); i++) {
      // if the key is found return the index
      if (dict[i][pos] == key) {
        return i;
      }
    }
    // if the key is not found return -1
    return -1;
  }
  // add operator []
  unsigned *operator[](unsigned index) {
    // return the unsigned pointer at the index
    return dict[index];
  }
  // return the size of the vector convert it unsinged
  unsigned size() { return (unsigned)dict.size(); }
};


void test_my_dict()
{
	// write some test
	Dict4X dict;
	dict.add(1, 2, 3, 4);
	dict.add(5, 6, 7, 8);
	dict.add(9, 10, 11, 12);
	dict.add(13, 14, 15, 16);
	
	// test search function
	assert(dict.search(1, 0) == 0);
	assert(dict.search(2, 1) == 0);
	assert(dict.search(3, 2) == 0);
	assert(dict.search(4, 3) == 0);
	assert(dict.search(5, 0) == 1);
	assert(dict.search(6, 1) == 1);
	assert(dict.search(7, 2) == 1);
	assert(dict.search(8, 3) == 1);
	assert(dict.search(9, 0) == 2);
	
	// test remove function
	dict.remove(0);
	assert(dict.search(1, 0) == -1);
	assert(dict.search(2, 1) == -1);
	
	// test add function
	dict.add(17, 18, 19, 20);
	assert(dict.search(17, 0) == 3);

	// test add function using pointer
	unsigned key[4] = { 21, 22, 23, 24 };
	dict.add(key);
	assert(dict.search(21, 0) == 4);

	// test operator []
	assert(dict[0][0] == 5);
	assert(dict[0][1] == 6);
	assert(dict[0][2] == 7);
	assert(dict[0][3] == 8);

	// test size function
	assert(dict.size() == 5);
	
}
