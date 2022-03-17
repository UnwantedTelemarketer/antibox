#include "sprite.h"
#include <antibox/graphics/rendercommands.h>
#include "antibox/core/engine.h"
#include <iostream>

namespace antibox 
{
	Sprite::Sprite() {
		Engine::Instance().AddToSprites(this);
	}
	void Sprite::CreateSprite() {
		if (alreadyMade) { std::cout << "Already created this sprite!" << "\n"; return; }
		float vertices[]
		{
			0.5f,  0.5f, 0.f,
			0.5f, -0.5f, 0.f,
			-0.5f, -0.5f, 0.f,
			-0.5f,  0.5f, 0.f
		};
		mMesh = std::make_shared<Mesh>(&vertices[0], 3, 3);

		mShader = std::make_shared<Shader>(defaultVert, defaultFrag);
		mShader->SetUniformFloat3("color", 1, xColor, yColor);

		mRectPos = glm::vec2(0.f);
		mRectSize = glm::vec2(1.f);

		alreadyMade = true;
	}

	void Sprite::UpdateSprite() {
		mShader->SetUniformFloat2("color", xColor, yColor);

		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, { mRectPos.x, mRectPos.y, 0.f });
		model = glm::scale(model, { mRectSize.x, mRectSize.y, 0.f });
		mShader->SetUniformMat4("model", model);
	}

	void Sprite::RenderSprite() {
		auto rc = std::make_unique<render::RenderMesh>(mMesh, mShader);
		Engine::Instance().GetRenderManager().Submit(std::move(rc));
		Engine::Instance().GetRenderManager().Flush();
	}
}