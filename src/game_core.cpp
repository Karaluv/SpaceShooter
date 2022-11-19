#define _CRTDBG_MAP_ALLOC

#define enable_testing

#include <stdexcept>
#include <limits>
#include <crtdbg.h>
#include <stdlib.h>

#include <chrono>
#include <iostream>
#include <thread>

#include <render.cpp>
#include <serg_main.cpp>

#include <mynum.cpp>
#include <MyDict.cpp>

#include "globals.hpp"


#ifdef enable_testing
void Run_tests()
{
    // test_my_num();
	test_my_dict();

}
#endif

int main() {
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

    #ifdef enable_testing
    // run tests
        Run_tests();
    #endif


  srs::start_render();
  {
    // create monkey and cube and several lights in different positions and
    // angels
    srs::create_object("monkey", 0, 0, -1, 0, 0, 0, 1);
    srs::create_object("cube", 4, 0, -1, 0, 0, 0, 1);

    // now I need to create a light
    srs::create_light(8, 3, -2, 1, 1, 1, 50);
    srs::create_light(-8, -3, -2, 1, 1, 1, 50);

    // write some code 144 times per seconds to update camera position,
    // objects position, lights position and color
    for (int i = 0; i < 10000; i++) {
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
