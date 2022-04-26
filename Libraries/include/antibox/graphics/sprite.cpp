#include "sprite.h"
#include <antibox/graphics/rendercommands.h>
#include "antibox/core/engine.h"
#include <iostream>

namespace antibox 
{
	void Sprite::CreateSprite() {
		

		

		mRectPos = glm::vec2(0.f);
		mRectSize = glm::vec2(1.f);

		alreadyMade = true;
	}

	void Sprite::UpdateSprite(const glm::vec2& pos, const glm::vec2& size) {
		mShader->SetUniformFloat2("color", 250.0f, 250.0f);

		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, { pos.x, pos.y, 0.f });
		model = glm::scale(model, { size.x, size.y, 0.f });
		mShader->SetUniformMat4("model", model);
	}

	void Sprite::RenderSprite() {
		auto rc = std::make_unique<render::RenderMesh>(mMesh, mShader);
		Engine::Instance().GetRenderManager().Submit(std::move(rc));
		Engine::Instance().GetRenderManager().Flush();
	}
}