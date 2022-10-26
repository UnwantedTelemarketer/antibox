#pragma once

#include <memory>
#include "../objects/scene.h"
#include "../graphics/texture.h"

namespace Factory 
{
	static float Vertices[]
	{
		 0.5f,  0.5f, 0.f,
		 0.5f, -0.5f, 0.f,
		-0.5f, -0.5f, 0.f,
		-0.5f,  0.5f, 0.f
	};

	static uint32_t Elements[]
	{
		0, 3, 1,
		1, 3, 2
	};

	static float texcoords[]
	{
		1.f, 1.f,
		1.f, 0.f,
		0.f, 0.f,
		0.f, 1.f,
	};

	static const char* DefaultVert = R"(
			#version 410 core
			layout (location = 0) in vec3 position;
			layout (location = 1) in vec2 texcoords;
			uniform mat4 model = mat4(1.0);
			out vec2 uvs;

			void main()
			{
				uvs = texcoords;
				gl_Position = model * vec4(position,1.0);
			}
		)";

	static const char* DefaultFrag = R"(
			#version 410 core
			out vec4 outColor;
			in vec2 uvs;

			uniform sampler2D tex;
			void main()
			{
				//outColor = vec4(1.0);
				outColor = texture(tex, uvs);
			}
		)";

	std::shared_ptr<antibox::GameObject> CreateSprite(const glm::vec2& pos)
	{
		std::shared_ptr<antibox::Mesh> mesh = std::make_shared<antibox::Mesh>(&Vertices[0], 4, 3, &Elements[0], 6);
		std::shared_ptr<antibox::Shader> shader = std::make_shared<antibox::Shader>(DefaultVert, DefaultFrag);

		glm::vec2 size{ 0.5,0.5 };
		auto ret = std::make_shared<antibox::GameObject>(mesh, shader, pos, size);
		return ret;
	}
}