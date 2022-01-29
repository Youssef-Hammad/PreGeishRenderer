#include "RenderableObject.h"

void RenderableObject::load_vertices()
{
	// Counting number of vertices from faces
	for (int i = 0; i < mesh->face_count; i++)
	{
		if (mesh->face_vertices[i] == 3)
			vertices_size += 3;
		else if (mesh->face_vertices[i] == 4) // faces with 4 vertices (rectangle) are sliced up to make 2 triangles
			vertices_size += 6;
	}

	//Calculating the total number of floats in the vertices array (the ternary operations multiply the number of vertices with the number of floats per single vertix
	int temp_vertices_size = vertices_size * ((mesh->position_count ? 3 : 1) + (mesh->texcoord_count ? 2 : 1) + (mesh->normal_count ? 3 : 1));
	vertices = new float[temp_vertices_size];

	// current element in the indices array in fastObjMesh object
	int indicies_idx = 0;

	// law somehow nasy el offset eh lazmeto bsabab el gesh
	// each vertex position contains 3 floats, so when you access the vertex position i
	// the 3 floating values will start at i*3
	// Now apply the same concept to the texture and normal
	const int position_offset = 3;
	const int texture_offset = 2;
	const int normal_offset = 3;

	// current index in array "vertices"
	int current_vertices_idx = 0;

	// The purpose of this loop is to populate the array "vertices" with the 3D object's data
	// The array is populated in the following format :
	//										Vertex 1 : PosX PosY PosZ TexX TexY NormalX NormalY NormalZ
	//										Vertex 2 : etc...
	for (int i = 0; i < mesh->face_count; i++)
	{
		if (mesh->face_vertices[i] == 3)
		{
			int vertix_end = indicies_idx + 3;
			for (; indicies_idx < vertix_end; indicies_idx++)
			{
				// offset * index indicates the beginning of the vertex position/texture/normal co-ordiantes

				// Position
				vertices[current_vertices_idx++] = mesh->positions[position_offset * mesh->indices[indicies_idx].p];
				vertices[current_vertices_idx++] = mesh->positions[position_offset * mesh->indices[indicies_idx].p + 1];
				vertices[current_vertices_idx++] = mesh->positions[position_offset * mesh->indices[indicies_idx].p + 2];

				// Texture coordinates
				vertices[current_vertices_idx++] = mesh->texcoords[texture_offset * mesh->indices[indicies_idx].t];
				vertices[current_vertices_idx++] = mesh->texcoords[texture_offset * mesh->indices[indicies_idx].t + 1];

				// Normals
				vertices[current_vertices_idx++] = mesh->normals[normal_offset * mesh->indices[indicies_idx].n];
				vertices[current_vertices_idx++] = mesh->normals[normal_offset * mesh->indices[indicies_idx].n + 1];
				vertices[current_vertices_idx++] = mesh->normals[normal_offset * mesh->indices[indicies_idx].n + 2];
			}
		}
		else if (mesh->face_vertices[i] == 4)
		{
			// 0------------1
			// | \        /	|
			// |  \      /	|
			// |   \	/	|
			// |	\  /	|
			// |	 \/		|
			// |	 /\		|
			// |	/  \	|
			// |   /	\	|
			// |  /		 \	|
			// | /		  \	|
			// |/		   \|
			// 2------------3
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
	// counter for the number of vertices that sample from current_texture_idx
	int vertices_using_current_texture = 0;
	// Current texture id (that -1 is to make sure it's not set before)
	int current_texture_idx = -1;

	// Loop that populates the "texture_order" vector 
	// (check the comment on the vector in RenderableObject.h for more info)
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
		
		if(i==mesh->face_count-1) // handling pushing back when reaching the end of the faces
			texture_order.push_back({ vertices_using_current_texture, current_texture_idx });
	}

	// Loop that loads the material info into "textures" vector
	for (int i = 0; i < mesh->material_count; i++)
	{
		glm::vec3 ambient = glm::vec3(mesh->materials[i].Ka[0], mesh->materials[i].Ka[1], mesh->materials[i].Ka[2]);
		glm::vec3 diffuse = glm::vec3(mesh->materials[i].Kd[0], mesh->materials[i].Kd[1], mesh->materials[i].Kd[2]);
		glm::vec3 specular = glm::vec3(mesh->materials[i].Ks[0], mesh->materials[i].Ks[1], mesh->materials[i].Ks[2]);

		//				TextureId				Diffuse Texture									not currently used		Shininess (Check fragment shader for more info)
		Texture temp((GL_TEXTURE0+i),mesh->materials[i].map_Kd.path,ambient,diffuse,specular,mesh->materials[i].illum,mesh->materials[i].Ns);
		textures.push_back(temp);
	}
}

RenderableObject::RenderableObject(std::string path, Shader* _shaderProgram, glm::vec3 position)
	: mesh(fast_obj_read(path.c_str())), vertices_size(0) ,shaderProgram(_shaderProgram), Position(position)
{
	load_vertices();

	load_textures();
	
	// Construction of the VBO
	// Law nasy el syntax me7tag tet3alem OpenGL mel awl https://learnopengl.com/Getting-started/Hello-Triangle
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	// 8 = 3 vertex positions + 3 vertex normals + 2 texture coordinates
	int stride_size = 8;

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices_size * stride_size * sizeof(float), vertices, GL_STATIC_DRAW);

	// Position (location 0)
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride_size * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// Texture Coordinates (location 1)
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, stride_size * sizeof(float), (void*)((mesh->position_count ? 3 : 1)  * sizeof(float)));
	glEnableVertexAttribArray(1);

	// Normals (location 2)
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, stride_size * sizeof(float), (void*)(((mesh->position_count ? 3 : 1) + (mesh->texcoord_count ? 2 : 1)) * sizeof(float)));
	glEnableVertexAttribArray(2);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);


}

void RenderableObject::draw()
{
	// Binding the object's VAO and the shader program it uses
	glBindVertexArray(VAO);
	shaderProgram->SetActive();


	int vertices_cnt = 0; // acts as an offset for the number of vertices that have already been drawn
	
	// Translating the model's position in the world
	// For now I have set it to rotate all the objects 180 around the y axis (Yaw)
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, Position);
	model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	shaderProgram->setMat4("model", model);

	// Looping over texture_order
	// Each element sets the specific material data (according to the second int in texture_order)
	// And then issues a draw call to draw the number of vertices (according to the first int in texture_order)
	// Then increases the offset vertices_cnt to keep track of the start of the next draw
	for (int i = 0; i < texture_order.size(); i++)
	{
		// Binding the texture and setting the appropriate uniform data
		textures[texture_order[i].second].bind();
		shaderProgram->setInt("material.diffuseTex", texture_order[i].second);
		shaderProgram->setVec3("material.ambient", textures[texture_order[i].second].material.ambient);
		shaderProgram->setVec3("material.diffuse", textures[texture_order[i].second].material.diffuse);
		shaderProgram->setVec3("material.specular", textures[texture_order[i].second].material.specular);
		shaderProgram->setFloat("material.shininess", textures[texture_order[i].second].material.shininess);

		// This part is a bit unorganized and the logic needs to be revisited
		if (textures[texture_order[i].second].material.illumModel == 2)
		{
			shaderProgram->setInt("material.specularTex", texture_order[i].second);
		}
		glDrawArrays(GL_TRIANGLES, vertices_cnt, texture_order[i].first);
		// Increasing vertices_cnt to be correctly positioned as an offset to where to start
		// When drawing vvertices
		vertices_cnt += texture_order[i].first;
		textures[texture_order[i].second].unbind();
	}
	glBindVertexArray(0);
}

RenderableObject::~RenderableObject()
{
	fast_obj_destroy(mesh);
	delete[] vertices;
	
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
}
