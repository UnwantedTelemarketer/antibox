#pragma once
#include <memory>

#include "antibox/graphics/shader.h"
#include "antibox/graphics/mesh.h"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

namespace antibox {
	class Sprite {
	private:
		float xColor;
		float yColor;
		std::shared_ptr<Mesh> mMesh;
		std::shared_ptr<Shader> mShader;
		glm::vec2 mRectPos, mRectSize;

		const char* defaultVert = R"(
			#version 410 core
			layout (location = 0) in vec3 position;
			out vec3 vpos;
			uniform mat4 model = mat4(1.0);
			void main()
			{
				vpos = position;
				gl_Position = model * vec4(position,1.0);
			}
		)";
		const char* defaultFrag = R"(
			#version 410 core
			out vec4 outColor;
			in vec3 vpos;

			uniform vec3 color = vec3(0.0);
			void main()
			{
				outColor = vec4(1.0,1.0,1.0, 1.0);
			}
		)";
		bool alreadyMade = false;

	public:
		void CreateSprite();
		void UpdateSprite();
		void RenderSprite();

		Sprite() {}
	};
}