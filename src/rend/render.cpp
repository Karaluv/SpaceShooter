#include <iostream>
#include <render_engine.cpp>
#include <thread>
#include <chrono>
namespace srs

{
	render_engine *RendEng = new render_engine();

	void initialize_all()
	{
		std::cout << "Render Engine was initialized successfully!\n";
	}
	void start_render()
	{
		RendEng->start();
	}
	void update_data()
	{
	}
	void tmp_func()
	{
		while (!RendEng->GetInitialize())
			std::this_thread::sleep_for(std::chrono::nanoseconds(100));
		RendEng->new_object("cube", 1.0, 2.0, 3.0, 0.0, 0.0, 0.0, 0.0);
		double x, y, z;
		x = 1.0;
		y = 2.0;
		z = 3.0;
		while (true)
		{
			x += 0.002;
			y -= 0.002;
			z -= 0.002;
			RendEng->update_obj(0, x, y, z, 0.0, 0.0, 0.0, 0.0);
			std::this_thread::sleep_for(std::chrono::nanoseconds(1000000000 / 144));
		}
	}
	void terminate_render()
	{
		std::cout << "Render Engine was terminated successfully!\n";
	}
}