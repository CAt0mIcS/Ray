#include "Rpch.h"
#include "RTextRenderer.h"

#include "Utils/RAssert.h"

#include "RMesh.h"
#include "RTransform.h"
#include "Graphics/Text/RFont.h"
#include "Shading/Flat/RFlatTextMaterial.h"
#include "Graphics/Images/RTexture2DAtlas.h"
#include "Graphics/Pipelines/Shader/RShader.h"
#include "Graphics/Pipelines/Uniforms/RDescriptor.h"
#include "Graphics/Pipelines/Uniforms/RBufferUniform.h"


namespace At0::Ray
{
	TextRenderer::TextRenderer(Entity entity, Ref<FlatTextMaterial> material)
		: Component(entity), Renderer(material)
	{
		AddBufferUniform("PerObjectData", ShaderStage::Vertex);
		AddSampler2DUniform("samplerCharacters", ShaderStage::Fragment,
			material->GetFont().GetSharedTextureAtlas());
		GetEntity().Get<Transform>().Rotate({ /*Math::PI<>*/ 0.0f, 0.0f, 0.0f });
	}

	void TextRenderer::Update()
	{
		if (auto& tform = GetEntity().Get<Transform>(); tform.HasChanged())
		{
			if (GetEntity().HasParent())
				GetBufferUniform("PerObjectData")["Model"] =
					GetEntity().GetParent().Get<Transform>().AsMatrix() * tform.AsMatrix();
			else
				GetBufferUniform("PerObjectData")["Model"] = tform.AsMatrix();
		}
	}

	void TextRenderer::Render(const CommandBuffer& cmdBuff) const
	{
		m_Material->CmdBind(cmdBuff);

		for (const auto& descSet : m_DescriptorSets)
			descSet.CmdBind(cmdBuff);

		GetEntity().Get<Mesh>().CmdBind(cmdBuff);
	}
}  // namespace At0::Ray
