#include "Timer.h"
#include "Initializer.h"

int main()
{
	/*Initialize();


	{
		Timer timer;
		renderer.AddObj("test_resources\\3D-Models\\RubiksCube\\rubikscube.obj", glm::vec3(0.0f, 0.0f, 0.0f));
		std::cout << "Rubiks cube loading time:\n";
	}
	std::cout << "\n\n";
	{
		Timer timer;
		renderer.AddObj("test_resources\\3D-Models\\Antylamon Snow\\Antylamon Snow.obj", glm::vec3(3.0f, 0.0f, 0.0f));
		std::cout << "Antylamon Snow loading time:\n";
	}
	std::cout << "\n\n";
	{
		Timer timer;
		renderer.AddObj("test_resources\\3D-Models\\Taichi\\digimon-digital-monsters-tai.obj", glm::vec3(6.0f, 0.0f, 0.0f));
		std::cout << "Taichi loading time:\n";
	}
	std::cout << "\n\n";
	{
		Timer timer;
		renderer.AddObj("test_resources\\3D-Models\\Electro Mage\\Electro Cicin Mage.obj", glm::vec3(9.0f, 0.0f, 0.0f));
		std::cout << "Cicin Mage loading time:\n";
	}
	std::cout << "\n\n";
	{
		Timer timer;
		renderer.AddObj("test_resources\\3D-Models\\AmongUs\\untitled.obj", glm::vec3(12.0f, 0.0f, 0.0f));
		std::cout << "Among us loading time:\n";
	}
	std::cout << "\n\n";
	{
		Timer timer;
		renderer.AddTerrain();
		std::cout << "Terrain loading time:\n";
	}

	renderer.ToggleSkyBox();*/

	Initialize();

	renderer.AddObj("test_resources\\3D-Models\\RubiksCube\\rubikscube.obj", glm::vec3(0.f, 0.f, 0.f));
	renderer.AddTerrain();
	renderer.ToggleSkyBox();

	while (WindowIsOpen())
	{

		renderer.render_scene();
	}

	Terminate();


	return 0;
}