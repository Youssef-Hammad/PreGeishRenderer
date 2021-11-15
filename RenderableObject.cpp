#include "RenderableObject.h"

void RenderableObject::load_vertices()
{
	// Counting number of vertices from faces
	for (int i = 0; i < mesh->face_count; i++)
	{
		if (mesh->face_vertices[i] == 3)
			vertices_size += 3;
		else if (mesh->face_vertices[i] == 4)
			vertices_size += 6;
	}

	//Calculating the total number of floats in the vertices array (the ternary operations multiply the number of vertices with the number of floats per single vertix
	int temp_vertices_size = vertices_size*3; //vertices_size * ((mesh->position_count ? 3 : 1) + (mesh->texcoord_count ? 2 : 1) + (mesh->normal_count ? 3 : 1));
	vertices = new float[temp_vertices_size];

	int indicies_idx = 0;

	const int position_offset = 3;
	const int texture_offset = 2;
	const int normal_offset = 3;

	int current_vertices_idx = 0;

	for (int i = 0; i < mesh->face_count; i++)
	{
		if (mesh->face_vertices[i] == 3)
		{
			int vertix_end = indicies_idx + 3;
			for (; indicies_idx < vertix_end; indicies_idx++)
			{
				// offset * index indicates the beginning of the vertex position/texture/normal co-ordiantes
				vertices[current_vertices_idx++] = mesh->positions[position_offset * mesh->indices[indicies_idx].p];
				vertices[current_vertices_idx++] = mesh->positions[position_offset * mesh->indices[indicies_idx].p + 1];
				vertices[current_vertices_idx++] = mesh->positions[position_offset * mesh->indices[indicies_idx].p + 2];

				/*vertices[current_vertices_idx++] = mesh->texcoords[texture_offset * mesh->indices[indicies_idx].t];
				vertices[current_vertices_idx++] = mesh->texcoords[texture_offset * mesh->indices[indicies_idx].t + 1];

				vertices[current_vertices_idx++] = mesh->normals[normal_offset * mesh->indices[indicies_idx].n];
				vertices[current_vertices_idx++] = mesh->normals[normal_offset * mesh->indices[indicies_idx].n + 1];
				vertices[current_vertices_idx++] = mesh->normals[normal_offset * mesh->indices[indicies_idx].n + 2];*/
			}
		}
		else if (mesh->face_vertices[i] == 4)
		{
			const int indicies_order[] = { 0,1,2,0,2,3 };

			//Splitting 4-index faces into 2 3-index faces
			for (int j = 0; j < 6; j++)
			{
				int temp_indicies_idx = indicies_idx + indicies_order[j];
				vertices[current_vertices_idx++] = mesh->positions[position_offset * mesh->indices[temp_indicies_idx].p];
				vertices[current_vertices_idx++] = mesh->positions[position_offset * mesh->indices[temp_indicies_idx].p + 1];
				vertices[current_vertices_idx++] = mesh->positions[position_offset * mesh->indices[temp_indicies_idx].p + 2];

				/*vertices[current_vertices_idx++] = mesh->texcoords[texture_offset * mesh->indices[temp_indicies_idx].t];
				vertices[current_vertices_idx++] = mesh->texcoords[texture_offset * mesh->indices[temp_indicies_idx].t + 1];

				vertices[current_vertices_idx++] = mesh->normals[normal_offset * mesh->indices[temp_indicies_idx].n];
				vertices[current_vertices_idx++] = mesh->normals[normal_offset * mesh->indices[temp_indicies_idx].n + 1];
				vertices[current_vertices_idx++] = mesh->normals[normal_offset * mesh->indices[temp_indicies_idx].n + 2];*/
			}
			indicies_idx += 4;
		}
	}
	//for (int i = 0; i < temp_vertices_size; i += 3)
		//std::cout << vertices[i] << " " << vertices[i + 1] << " " << vertices[i + 2] << std::endl;
}

RenderableObject::RenderableObject(std::string path, Shader* _shaderProgram, glm::vec3 position)
{
	//Initializations

	Position = position;
	mesh = fast_obj_read(path.c_str());
	shaderProgram = _shaderProgram;
	vertices_size = 0;


	load_vertices();
	
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices_size * 3 * sizeof(float), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);


}

void RenderableObject::draw()
{
	glBindVertexArray(VAO);
	shaderProgram->SetActive();
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, Position);
	model = glm::rotate(model, glm::radians(0.0f), glm::vec3(1.0f, 0.3f, 0.5f));
	shaderProgram->setMat4("model", model);
	glDrawArrays(GL_TRIANGLES, 0, vertices_size * 3);
}

RenderableObject::~RenderableObject()
{
}
