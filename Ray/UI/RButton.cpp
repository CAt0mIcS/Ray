#include "RButton.h"

#include "Components/RMeshRenderer.h"
#include "Graphics/Pipelines/Shader/DataAccess/RBufferUniform.h"
#include "Graphics/Pipelines/Shader/DataAccess/RSampler2DUniform.h"

#include "Graphics/RGraphics.h"
#include "Graphics/Core/RLogicalDevice.h"


namespace At0::Ray
{
	Button::Button(Entity entity, std::string_view name, Float2 pos, float width, float height,
		const Float3& color)
		: Widget(entity, name), m_UseColorMaterial(true)
	{
		// FlatColorMaterial::Layout layout{};
		// GraphicsPipeline::Layout pipelineLayout{};
		// pipelineLayout.cullMode = VK_CULL_MODE_NONE;
		// pipelineLayout.shader =
		//	Shader::AcquireSourceFile({ "Resources/Shaders/UI.vert", "Resources/Shaders/UI.frag" });
		// Ref<Material> material =
		//	MakeRef<FlatColorMaterial>(std::move(layout), std::move(pipelineLayout));

		// GetEntity().Emplace<Mesh>(Mesh::Plane(material));
		// MeshRenderer& renderer = GetEntity().Emplace<MeshRenderer>(std::move(material));
		// BufferUniform& uColor = renderer.AddBufferUniform("Shading", ShaderStage::Fragment);
		// uColor["color"] = color;

		SetWidth(width);
		SetHeight(height);
		SetTranslation(pos);
	}

	Button::Button(Entity entity, std::string_view name, Float2 pos, float width, float height,
		Ref<Texture> texture)
		: Widget(entity, name), m_UseColorMaterial(false)
	{
		// FlatTextureMaterial::Layout layout{};
		// GraphicsPipeline::Layout pipelineLayout{};
		// pipelineLayout.cullMode = VK_CULL_MODE_NONE;
		// pipelineLayout.shader =
		//	Shader::AcquireSourceFile({ "Resources/Shaders/UI_Tex.vert",
		//"Resources/Shaders/UI_Tex.frag" });
		// Ref<Material> material =
		//	MakeRef<FlatTextureMaterial>(std::move(layout), std::move(pipelineLayout));

		// GetEntity().Emplace<Mesh>(Mesh::Plane(material));
		// MeshRenderer& renderer = GetEntity().Emplace<MeshRenderer>(std::move(material));
		// renderer.AddSampler2DUniform("texSampler", ShaderStage::Fragment, std::move(texture));

		SetWidth(width);
		SetHeight(height);
		SetTranslation(pos);
	}

	void Button::SetColor(const Float3& color)
	{
		RAY_MEXPECTS(m_UseColorMaterial, "[Button] RAY_TODO: Cannot set color on textured button");
		GetEntity().Get<MeshRenderer>().GetBufferUniform("Shading")["color"] = color;
	}

	void Button::SetTexture(Ref<Texture> texture)
	{
		RAY_MEXPECTS(
			!m_UseColorMaterial, "[Button] RAY_TODO: Cannot set texture on colored button");

		MeshRenderer& renderer = GetEntity().Get<MeshRenderer>();

		// Wait for command buffers to finish executing because command buffers which are in the
		// pending state and are using the descriptor set need to finish before it can be updated
		vkQueueWaitIdle(Graphics::Get().GetDevice().GetGraphicsQueue());
		renderer.GetSampler2DUniform("texSampler")
			.SetTexture(std::move(texture), renderer.GetDescriptorSet("texSampler"));
	}
}  // namespace At0::Ray
