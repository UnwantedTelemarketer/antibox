#include "antibox/core/antibox.h"

#include "antibox/graphics/framebuffer.h"
using namespace antibox;
class _NAME_ : public App {
private:
	
public:
	Sprite triangleSprite;
	void Init() override 
	{
	} 
	void Update() override 
	{
	}
	void Render() override 
	{
	} 
	void ImguiRender() override
	{
		ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());
		ImGui::ShowDemoWindow();
		if (ImGui::Begin("Game View"))
		{
			auto& window = *Engine::Instance().GetWindow();
			ImGui::Image((void*)window.GetFramebuffer()->GetTextureID(), { ImGui::GetWindowSize().x , ImGui::GetWindowSize().y });
		}
		ImGui::End();
	}
	void Shutdown() override 
	{
	}
};

antibox::App* CreateApp() {
	return new _NAME_();
}