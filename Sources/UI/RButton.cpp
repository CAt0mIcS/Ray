#include "Rpch.h"
#include "RButton.h"

#include "Scene/RScene.h"
#include "Geometry/RMesh.h"
#include "Graphics/Pipelines/RGraphicsPipeline.h"
#include "Core/RMath.h"


namespace At0::Ray
{
	Button::Button(Entity entity, std::string_view name, Float2 pos, float width, float height)
		: Widget(entity, name)
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

		SetWidth(width);
		SetHeight(height);
		SetTranslation(pos);
	}

	void Button::SetColor(const Float3& color)
	{
		GetEntity().Get<Mesh>().GetMaterial().GetUniform<BufferUniform>("Shading")["color"] = color;
	}
}  // namespace At0::Ray
