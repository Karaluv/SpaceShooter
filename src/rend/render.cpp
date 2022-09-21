
#include <iostream>
#include <render_engine.cpp>

void initialize()
{
	initializeR();
	run();

	std::cout << "Initialize render" << '\n';
}