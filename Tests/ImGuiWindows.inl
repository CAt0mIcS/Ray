#if RAY_ENABLE_IMGUI
{
	ImGui::Begin("Scene");
	ImGui::Text("Stats");
	ImGui::Text(At0::Ray::String::Serialize(
		"Frametime: {0}ms", GetEngine().GetDelta("MainWindow").AsMilliseconds())
					.c_str());
	ImGui::Text(At0::Ray::String::Serialize("FPS: {0}", GetEngine().GetFPS("MainWindow")).c_str());
	ImGui::End();
}
#endif
