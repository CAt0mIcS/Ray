#include "Rpch.h"
#include "RButton.h"

#include "Scene/RScene.h"
#include "Geometry/RMesh.h"
#include "Graphics/Pipelines/RGraphicsPipeline.h"
#include "Core/RMath.h"


namespace At0::Ray
{
	Button::Button(Entity entity, Float2 pos) : Widget(entity)
	{
		// clang-format off
		Mesh& mesh = GetEntity().Emplace<Mesh>(Mesh::Plane(
			Material
			{
				Material::Shaders("Resources/Shaders/UITestShader.vert", "Resources/Shaders/UITestShader.frag"),
				Material::CullMode(VK_CULL_MODE_NONE)
			}));
		// clang-format on

		BufferUniform& uColor =
			(BufferUniform&)mesh.GetMaterial().AddUniform(MakeScope<BufferUniform>(
				"Shading", Shader::Stage::Fragment, mesh.GetMaterial().GetGraphicsPipeline()));
		uColor["color"] = Float3{ 1.0f, 1.0f, 1.0f };

		SetWidth(100);
		SetHeight(100);

		SetTranslation({ 0.0f, 0.0f });
	}
}  // namespace At0::Ray
