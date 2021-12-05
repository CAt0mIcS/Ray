#if RAY_ENABLE_IMGUI
Ray::ImGUI::Get().RegisterNewFrameFunction(
	[this]()
	{
		ImGui::Begin("Scene");
		ImGui::Text("Stats");
		ImGui::Text(Ray::String::Serialize(
			"Frametime: {0}ms", Ray::Engine::Get().GetDelta().AsMilliseconds())
						.c_str());
		ImGui::Text(Ray::String::Serialize("FPS: {0}", Ray::Engine::Get().GetFPS()).c_str());
		ImGui::Spacing();
		ImGui::Checkbox("Freeze Frustum", &Ray::Scene::Get().GetCamera().FreezeFrustum);
		ImGui::End();
	});
#endif
