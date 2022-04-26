#pragma once

#include "../graphics/sprite.h"
#include <memory>

namespace antibox 
{
	class GameObject {
	public:
		GameObject(std::shared_ptr<Mesh> mesh, std::shared_ptr<Shader> shader, const glm::vec2& pos, const glm::vec2& size)
			: mMesh(mesh)
			, mShader(shader)
			, mPos(pos)
			, mSize(size)
		{}

		void SetPos(const glm::vec2& pos) { mPos = pos; }
		void Move(const glm::vec2& distance) { mPos += distance; }

		const glm::vec2& GetPos() const { return mPos; }
		const glm::vec2& GetSize() const { return mSize; }

		void Update() {};
		void Render();

	private:
		std::shared_ptr<Mesh> mMesh;
		std::shared_ptr<Shader> mShader;
		glm::vec2 mPos, mSize;
	};
}