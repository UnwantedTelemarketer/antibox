#pragma once

#include <memory>
#include "../objects/scene.h"

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

	static const char* DefaultVert = R"(
			#version 410 core
			layout (location = 0) in vec3 position;
			uniform mat4 model = mat4(1.0);
			void main()
			{
				gl_Position = model * vec4(position,1.0);
			}
		)";

	static const char* DefaultFrag = R"(
			#version 410 core
			out vec4 outColor;

			void main()
			{
				outColor = vec4(1.0);
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