#pragma once

#ifndef SCENE_H
#define SCENE_H

#include <vector>
#include "RenderableObject.h"
#include <glm/glm/glm.hpp>

class Scene
{
private:
	struct Vec3
	{
		float x;
		float y;
		float z;
	};
public:
	struct SceneObject //Supposed to contain multiple objects
	{
		Vec3 position;
		Vec3 texture; //texture x,y and ID
		Vec3 normal;
	};

	std::vector<SceneObject*> offset;
	unsigned int current_texture_id = 0;
	SceneObject* objects;
	SceneObject* currentOffset; // Points to the position that is right after the last added object
	Scene();
	void AddObject(RenderableObject& object, glm::vec3 position);


};

#endif // !SCENE_H
