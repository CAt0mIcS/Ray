#include "Rpch.h"
#include "RButton.h"

#include "Scene/RScene.h"
#include "Geometry/RMesh.h"
#include "Graphics/Pipelines/RGraphicsPipeline.h"
#include "Core/RMath.h"


namespace At0::Ray
{
	Button::Button(Entity entity, std::string_view name, Float2 pos, float width, float height,
		const Float3& color)
		: Widget(entity, name), m_UseColorMaterial(true)
	{
		// clang-format off
		Mesh& mesh = GetEntity().Emplace<Mesh>(Mesh::Plane(
			Material
			{
				Material::Shaders("Resources/Shaders/UI.vert", "Resources/Shaders/UI.frag"),
				Material::CullMode(VK_CULL_MODE_NONE)
			}));
		// clang-format on

		BufferUniform& uColor =
			(BufferUniform&)mesh.GetMaterial().AddUniform(MakeScope<BufferUniform>(
				"Shading", Shader::Stage::Fragment, mesh.GetMaterial().GetGraphicsPipeline()));
		uColor["color"] = color;

		SetWidth(width);
		SetHeight(height);
		SetTranslation(pos);
	}

	Button::Button(Entity entity, std::string_view name, Float2 pos, float width, float height,
		Ref<Texture2D> texture)
		: Widget(entity, name), m_UseColorMaterial(false)
	{
		// clang-format off
		Mesh& mesh = GetEntity().Emplace<Mesh>(Mesh::Plane(
			Material
			{
				Material::Shaders("Resources/Shaders/UI_Tex.vert", "Resources/Shaders/UI_Tex.frag"),
				Material::CullMode(VK_CULL_MODE_NONE)
			}, Vertex::Position3D | Vertex::TextureCoordinate));
		// clang-format on

		mesh.GetMaterial().AddUniform(MakeScope<SamplerUniform>("texSampler",
			Shader::Stage::Fragment, std::move(texture), mesh.GetMaterial().GetGraphicsPipeline()));

		SetWidth(width);
		SetHeight(height);
		SetTranslation(pos);
	}

	void Button::SetColor(const Float3& color)
	{
		RAY_MEXPECTS(m_UseColorMaterial, "[Button] RAY_TODO: Cannot set color on textured button");
		GetEntity().Get<Mesh>().GetMaterial().GetUniform<BufferUniform>("Shading")["color"] = color;
	}
}  // namespace At0::Ray
