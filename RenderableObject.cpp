#include "RenderableObject.h"

void RenderableObject::load_vertices()
{

	//Counting vertices from the faces
	for (int i = 0; i < mesh->face_count; i++)
	{
		if (mesh->face_vertices[i] == 3)
			vertixCount += 3;
		else if (mesh->face_vertices[i] == 4)
			vertixCount += 6;
	}

	//Calculating the total number of floats in the vertices array (the ternary operations multiply the number of vertices with the number of floats per single vertix
	int temp_vertices_size = vertixCount * ((mesh->position_count ? 3 : 1) + (mesh->texcoord_count ? 2 : 1) + (mesh->normal_count ? 3 : 1));
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

}

void RenderableObject::load_textures()
{
	int vertices_using_current_texture = 0;
	int current_texture = -1;

	for (int i = 0; i < mesh->face_count; i++)
	{
		//first iteration
		if (current_texture == -1)
			current_texture = mesh->face_materials[i];
		else if (current_texture != mesh->face_materials[i])
		{
			texture_order.push_back({ vertices_using_current_texture,current_texture });
			vertices_using_current_texture = 0;
			current_texture = mesh->face_materials[i];
		}
		if (mesh->face_vertices[i] == 3)
			vertices_using_current_texture += 3;
		else if (mesh->face_vertices[i] == 4)
			vertices_using_current_texture += 6;
	}
	texture_order.push_back({ vertices_using_current_texture, current_texture });

	for (int i = 0; i < mesh->material_count; i++)
	{
		glm::vec3 ambient = glm::vec3(mesh->materials[i].Ka[0], mesh->materials[i].Ka[1], mesh->materials[i].Ka[2]);
		glm::vec3 diffuse = glm::vec3(mesh->materials[i].Kd[0], mesh->materials[i].Kd[1], mesh->materials[i].Kd[2]);
		glm::vec3 specular = glm::vec3(mesh->materials[i].Ks[0], mesh->materials[i].Ks[1], mesh->materials[i].Ks[2]);

		Texture* temp = new Texture(mesh->materials[i].map_Kd.path, ambient, diffuse, specular, mesh->materials[i].Ns);
		textures.push_back(temp);
	}
}

RenderableObject::RenderableObject(std::string path)
{
	mesh = fast_obj_read(path.c_str());

	load_vertices();
	load_textures();
	
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	int temp_vertices_size = vertixCount * ((mesh->position_count ? 3 : 1) + (mesh->texcoord_count ? 2 : 1) + (mesh->normal_count ? 3 : 1));
	int stride_size = (mesh->position_count ? 3 : 1) + (mesh->texcoord_count ? 2 : 1) + (mesh->normal_count ? 3 : 1);

	glBufferData(0, temp_vertices_size * sizeof(float), vertices, GL_STATIC_DRAW);

	if (mesh->position_count)
	{
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride_size * sizeof(float), (void*)0); // vertex position
		glEnableVertexAttribArray(0);
	}
	if (mesh->texcoord_count)
	{
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, stride_size * sizeof(float), (void*)(3 * sizeof(float))); // vertex texture coordinates
		glEnableVertexAttribArray(1);
	}
	if (mesh->normal_count)
	{
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, stride_size * sizeof(float), (void*)(5 * sizeof(float))); // vertex normal
		glEnableVertexAttribArray(2);
	}

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

}

void RenderableObject::draw()
{
	int temp_current_vertix = 0;
	for (int i = 0; i < texture_order.size(); i++)
	{

	}

}

RenderableObject::~RenderableObject()
{
	fast_obj_destroy(mesh);
}
