#include "gameobject.h"
#include "../core/engine.h"

void antibox::GameObject::Render()
{
	glm::mat4 model = glm::mat4(1.f);
	model = glm::translate(model, { mPos.x, mPos.y, 0.f });
	model = glm::scale(model, { mSize.x, mSize.y, 1.f });
	mShader->SetUniformMat4("model", model);
	auto rc = std::make_unique<render::RenderMeshTextured>(mMesh, mTexture, mShader);
	antibox::Engine::Instance().GetRenderManager().Submit(std::move(rc));
}