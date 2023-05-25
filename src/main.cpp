#include "Initializer.h"

int main()
{

	Initialize();

	renderer.AddObj("test_resources\\3D-Models\\RubiksCube\\rubikscube.obj", glm::vec3(30.f, 0.f, 30.f));
	renderer.AddTerrain();
	renderer.ToggleSkyBox();

	while (WindowIsOpen())
	{

		renderer.render_scene();
	}

	Terminate();


	return 0;
}