#include "scene.h"

namespace antibox {


	void Scene::CreateObject(const glm::vec2& pos) {

	}

	//GameObject& Scene::FindObject(const std::string name) {
	//	return Hierarchy.find(name);
	//}

	Scene::Scene(std::string name) {
		sceneName = name;
		sceneID = 1;
	}

	Scene::~Scene() {

	}
}