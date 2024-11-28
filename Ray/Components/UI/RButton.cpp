#include "RButton.h"

#include "Graphics/Pipelines/RGraphicsPipeline.h"
#include "Graphics/Pipelines/Shader/RShader.h"

#include "Shading/RMaterial.h"

#include "../RMesh.h"
#include "../RMeshRenderer.h"


namespace At0::Ray
{
	Button::Button(Entity entity, Window& window, std::string_view name, Float2 pos, float width,
		float height, const Float4& color)
		: Widget(entity, window, name), m_UseColorMaterial(true)
	{
		auto pipeline = GraphicsPipeline::Builder()
							.SetShader(Shader::FromSourceFile(
								{ "Resources/Shaders/UI.vert", "Resources/Shaders/UI.frag" }))
							.SetCullMode(VK_CULL_MODE_NONE)
							.Acquire();

		auto material = Material::Builder(std::move(pipeline)).Set("Shading.color", color).Build();

		GetEntity().Emplace<Mesh>(Mesh::Plane(std::move(material)));

		SetWidth(width);
		SetHeight(height);
		SetTranslation(pos);
	}

	Button::Button(Entity entity, Window& window, std::string_view name, Float2 pos, float width,
		float height, Ref<Texture> texture)
		: Widget(entity, window, name), m_UseColorMaterial(false)
	{
		auto pipeline = GraphicsPipeline::Builder()
							.SetShader(Shader::FromSourceFile({ "Resources/Shaders/UI_Tex.vert",
								"Resources/Shaders/UI_Tex.frag" }))
							.SetCullMode(VK_CULL_MODE_NONE)
							.Acquire();

		auto material =
			Material::Builder(std::move(pipeline)).Set("texSampler", std::move(texture)).Build();
		GetEntity().Emplace<Mesh>(Mesh::Plane(std::move(material)));

		SetWidth(width);
		SetHeight(height);
		SetTranslation(pos);
	}

	void Button::SetColor(const Float4& color)
	{
		RAY_MEXPECTS(m_UseColorMaterial, "[Button] RAY_TODO: Cannot set color on textured button");
		GetEntity().Get<MeshRenderer>().GetMaterial().Set("Shading.color", color);
	}

	void Button::SetTexture(Ref<Texture> texture)
	{
		RAY_MEXPECTS(
			!m_UseColorMaterial, "[Button] RAY_TODO: Cannot set texture on colored button");

		Material& material = GetEntity().Get<MeshRenderer>().GetMaterial();

		// Wait for command buffers to finish executing because command buffers which are in the
		// pending state and are using the descriptor set need to finish before it can be updated
		// vkQueueWaitIdle(Graphics::Get().GetDevice().GetGraphicsQueue());

		// renderer.GetSampler2DUniform("texSampler")
		//	.SetTexture(std::move(texture), renderer.GetDescriptorSet("texSampler"));
		material.Set("texSampler", std::move(texture));
	}
}  // namespace At0::Ray
