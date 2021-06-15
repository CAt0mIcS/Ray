#include "Rpch.h"
#include "RButton.h"

#include "Shading/Flat/RFlatColorMaterial.h"
#include "Shading/Flat/RFlatTextureMaterial.h"
#include "Scene/RScene.h"
#include "Components/RMesh.h"
#include "Graphics/Pipelines/RGraphicsPipeline.h"
#include "Core/RMath.h"
#include "Graphics/RCodex.h"
#include "Components/RMeshRenderer.h"
#include "Graphics/Pipelines/Shader/RShader.h"


namespace At0::Ray
{
	Button::Button(Entity entity, std::string_view name, Float2 pos, float width, float height,
		const Float3& color)
		: Widget(entity, name), m_UseColorMaterial(true)
	{
		FlatColorMaterial::Layout layout{};
		GraphicsPipeline::Layout pipelineLayout{};
		pipelineLayout.cullMode = VK_CULL_MODE_NONE;
		pipelineLayout.shader = Codex::Resolve<Shader>(
			std::vector<std::string>{ "Resources/Shaders/UI.vert", "Resources/Shaders/UI.frag" });
		Ref<Material> material =
			MakeRef<FlatColorMaterial>(std::move(layout), std::move(pipelineLayout));

		GetEntity().Emplace<Mesh>(Mesh::Plane(material));
		MeshRenderer& renderer = GetEntity().Emplace<MeshRenderer>(std::move(material));
		BufferUniform& uColor = renderer.AddBufferUniform("Shading", ShaderStage::Fragment);
		uColor["color"] = color;

		SetWidth(width);
		SetHeight(height);
		SetTranslation(pos);
	}

	Button::Button(Entity entity, std::string_view name, Float2 pos, float width, float height,
		Ref<Texture2D> texture)
		: Widget(entity, name), m_UseColorMaterial(false)
	{
		FlatTextureMaterial::Layout layout{};
		GraphicsPipeline::Layout pipelineLayout{};
		pipelineLayout.cullMode = VK_CULL_MODE_NONE;
		pipelineLayout.shader = Codex::Resolve<Shader>(std::vector<std::string>{
			"Resources/Shaders/UI_Tex.vert", "Resources/Shaders/UI_Tex.frag" });
		Ref<Material> material =
			MakeRef<FlatTextureMaterial>(std::move(layout), std::move(pipelineLayout));

		GetEntity().Emplace<Mesh>(Mesh::Plane(material));
		MeshRenderer& renderer = GetEntity().Emplace<MeshRenderer>(std::move(material));
		renderer.AddSampler2DUniform("texSampler", ShaderStage::Fragment, std::move(texture));

		SetWidth(width);
		SetHeight(height);
		SetTranslation(pos);
	}

	void Button::SetColor(const Float3& color)
	{
		RAY_MEXPECTS(m_UseColorMaterial, "[Button] RAY_TODO: Cannot set color on textured button");
		GetEntity().Get<MeshRenderer>().GetBufferUniform("Shading")["color"] = color;
	}

	void Button::SetTexture(Ref<Texture2D> texture)
	{
		RAY_MEXPECTS(
			!m_UseColorMaterial, "[Button] RAY_TODO: Cannot set texture on colored button");

		MeshRenderer& renderer = GetEntity().Get<MeshRenderer>();

		renderer.GetSampler2DUniform("texSampler")
			.SetTexture(std::move(texture), renderer.GetDescriptorSet("texSampler"));
	}
}  // namespace At0::Ray
