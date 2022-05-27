#include "Scene.h"

Scene::Scene()
{
	objects = new SceneObject;
	currentOffset = objects;
}

void Scene::AddObject(RenderableObject& object, glm::vec3 position)
{
	int offset = 8; // each vertix contains 8 floats
	for (int i = 0; i < object.vertices_size; i++)
	{
		currentOffset[i].position.x = object.vertices[offset * i];
		currentOffset[i].position.y = object.vertices[offset * i + 1];
		currentOffset[i].position.z = object.vertices[offset * i + 2];

		currentOffset[i].texture.x = object.vertices[offset * i + 3];
		currentOffset[i].texture.y = object.vertices[offset * i + 4];
		
		currentOffset[i].normal.x = object.vertices[offset * i + 5];
		currentOffset[i].normal.y = object.vertices[offset * i + 6];
		currentOffset[i].normal.z = object.vertices[offset * i + 7];
	}

}
