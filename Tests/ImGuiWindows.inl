Ray::ImGUI::Get().RegisterNewFrameFunction([this]() {
	ImGui::Begin("Scene");
	ImGui::Checkbox("Freeze Frustum", &Scene::Get().GetCamera().FreezeFrustum);
	ImGui::End();
});
