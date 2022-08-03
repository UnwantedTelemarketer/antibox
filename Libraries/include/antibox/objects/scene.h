#pragma once

#include "gameobject.h"

namespace antibox {
	class Scene {
	private:
		int sceneID;
		std::string sceneName;
		std::unordered_map<std::string, GameObject&> Hierarchy;
	public:

		Scene(std::string name);
		~Scene();

		inline int GetSceneID() { return sceneID; }
		inline std::string GetSceneName() { return sceneName; }

		//Create new GameObject in this scene
		void CreateObject(const glm::vec2& pos);

		//Find GameObject in scene by name
		//GameObject& FindObject(const std::string name);
	};
}