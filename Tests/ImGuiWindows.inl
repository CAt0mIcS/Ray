#if RAY_ENABLE_IMGUI
At0::Ray::ImGUI::Get().RegisterNewFrameFunction(
	[this]()
	{
		ImGui::Begin("Scene");
		ImGui::Text("Stats");
		ImGui::Text(At0::Ray::String::Serialize(
			"Frametime: {0}ms", At0::Ray::Engine::Get().GetDelta().AsMilliseconds())
						.c_str());
		ImGui::Text(
			At0::Ray::String::Serialize("FPS: {0}", At0::Ray::Engine::Get().GetFPS()).c_str());
		ImGui::Spacing();
		ImGui::Checkbox("Freeze Frustum", &At0::Ray::Scene::Get().GetCamera().FreezeFrustum);
		ImGui::End();
	});
#endif
