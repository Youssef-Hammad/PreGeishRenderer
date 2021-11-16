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
	int temp_vertices_size = vertices_size * ((mesh->position_count ? 3 : 1) + (mesh->texcoord_count ? 2 : 1) + (mesh->normal_count ? 3 : 1));
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

				vertices[current_vertices_idx++] = mesh->texcoords[texture_offset * mesh->indices[indicies_idx].t];
				vertices[current_vertices_idx++] = mesh->texcoords[texture_offset * mesh->indices[indicies_idx].t + 1];

				vertices[current_vertices_idx++] = mesh->normals[normal_offset * mesh->indices[indicies_idx].n];
				vertices[current_vertices_idx++] = mesh->normals[normal_offset * mesh->indices[indicies_idx].n + 1];
				vertices[current_vertices_idx++] = mesh->normals[normal_offset * mesh->indices[indicies_idx].n + 2];
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

				vertices[current_vertices_idx++] = mesh->texcoords[texture_offset * mesh->indices[temp_indicies_idx].t];
				vertices[current_vertices_idx++] = mesh->texcoords[texture_offset * mesh->indices[temp_indicies_idx].t + 1];

				vertices[current_vertices_idx++] = mesh->normals[normal_offset * mesh->indices[temp_indicies_idx].n];
				vertices[current_vertices_idx++] = mesh->normals[normal_offset * mesh->indices[temp_indicies_idx].n + 1];
				vertices[current_vertices_idx++] = mesh->normals[normal_offset * mesh->indices[temp_indicies_idx].n + 2];
			}
			indicies_idx += 4;
		}
	}
	//for (int i = 0; i < temp_vertices_size; i += 3)
		//std::cout << vertices[i] << " " << vertices[i + 1] << " " << vertices[i + 2] << std::endl;
}

void RenderableObject::load_textures()
{
	int vertices_using_current_texture = 0;
	int current_texture_idx = -1;

	for (int i = 0; i < mesh->face_count; i++)
	{
		//first iteration
		if (current_texture_idx == -1)
			current_texture_idx = mesh->face_materials[i];
		else if (current_texture_idx != mesh->face_materials[i])
		{
			texture_order.push_back({ vertices_using_current_texture, current_texture_idx });
			vertices_using_current_texture = 0;
			current_texture_idx = mesh->face_materials[i];
		}
		if (mesh->face_vertices[i] == 3)
			vertices_using_current_texture += 3;
		else if (mesh->face_vertices[i] == 4)
			vertices_using_current_texture += 6;
	}
	texture_order.push_back({ vertices_using_current_texture, current_texture_idx });

	for (int i = 0; i < mesh->material_count; i++)
	{
		glm::vec3 ambient = glm::vec3(mesh->materials[i].Ka[0], mesh->materials[i].Ka[1], mesh->materials[i].Ka[2]);
		glm::vec3 diffuse = glm::vec3(mesh->materials[i].Kd[0], mesh->materials[i].Kd[1], mesh->materials[i].Kd[2]);
		glm::vec3 specular = glm::vec3(mesh->materials[i].Ks[0], mesh->materials[i].Ks[1], mesh->materials[i].Ks[2]);

		Texture* temp = new Texture(mesh->materials[i].map_Kd.path,ambient,diffuse,specular,mesh->materials[i].illum,mesh->materials[i].Ns);
		textures.push_back(temp);
	}
	std::cout << "TextureCnt: " << textures.size() << std::endl;
}

RenderableObject::RenderableObject(std::string path, Shader* _shaderProgram, glm::vec3 position)
{
	//Initializations

	Position = position;
	mesh = fast_obj_read(path.c_str());
	shaderProgram = _shaderProgram;
	vertices_size = 0;


	load_vertices();
	load_textures();
	
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	int stride_size = (mesh->position_count ? 3 : 1) + (mesh->texcoord_count ? 2 : 1) + (mesh->normal_count ? 3 : 1);

	std::cout << "Stride size: " << stride_size << std::endl;

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices_size*stride_size * sizeof(float), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride_size * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, stride_size * sizeof(float), (void*)((mesh->position_count ? 3 : 1)  * sizeof(float)));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, stride_size * sizeof(float), (void*)(((mesh->position_count ? 3 : 1) + (mesh->texcoord_count ? 2 : 1)) * sizeof(float)));
	glEnableVertexAttribArray(2);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);


}

void RenderableObject::draw()
{
	glBindVertexArray(VAO);
	shaderProgram->SetActive();
	int vertices_cnt = 0;
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, Position);
	model = glm::rotate(model, glm::radians(0.0f), glm::vec3(1.0f, 0.3f, 0.5f));
	shaderProgram->setMat4("model", model);
	for (int i = 0; i < texture_order.size(); i++)
	{
		//TODO: CHANGE TEXTURE BINDING TO COPE WITH THE NEW SHADER
		shaderProgram->setInt("material.diffuse", texture_order[i].second);
		/*if (textures[texture_order[i].second]->material.illumModel == 2)
		{
			shaderProgram->setInt("material.specular", texture_order[i].second);
			shaderProgram->setFloat("material.shininess", textures[texture_order[i].second]->material.shininess);
		}*/
		//shaderProgram->setInt("texture1", texture_order[i].second);
		textures[texture_order[i].second]->bind();
		//std::cout << textures[texture_order[i].second]->texture_number << std::endl;
		glDrawArrays(GL_TRIANGLES, vertices_cnt, texture_order[i].first);
		vertices_cnt += texture_order[i].first;
		textures[texture_order[i].second]->unbind();
	}
	//glDrawArrays(GL_TRIANGLES, 0, vertices_size * 3);
	glBindVertexArray(0);
}

RenderableObject::~RenderableObject()
{
	fast_obj_destroy(mesh);
	delete[] vertices;
	for (int i = 0; i < textures.size(); i++)
		delete textures[i];
}
