#include "Rpch.h"
#include "RTextRenderer.h"

#include "Utils/RAssert.h"
#include "Utils/RString.h"

#include "RMesh.h"
#include "RTransform.h"

#include "Graphics/RCodex.h"
#include "Graphics/Buffers/RVertexBuffer.h"
#include "Graphics/Buffers/RIndexBuffer.h"

#include "Graphics/Text/RFont.h"
//#include "Shading/Flat/RFlatTextMaterial.h"
#include "Graphics/Images/RTexture2DAtlas.h"
#include "Graphics/Pipelines/Shader/RShader.h"
#include "Graphics/Pipelines/Shader/DataAccess/RDescriptor.h"
#include "Graphics/Pipelines/Shader/DataAccess/RBufferUniform.h"


namespace At0::Ray
{
	// static Mesh::VertexData GeneratePlane(Ref<FlatTextMaterial> material, char charToRender)
	//{
	//	const Font::Glyph& glyph = material->GetFont().GetGlyph(charToRender);
	//	const Texture2DAtlas::Area& area = glyph.area;
	//	const Texture2DAtlas& textureAtlas = material->GetFont().GetTextureAtlas();

	//	std::array<Float2, 4> uvs = textureAtlas.MapUV(area);

	//	DynamicVertex vertex(material->GetGraphicsPipeline().GetShader());

	//	bool hasPos = vertex.Has(AttributeMap<AttributeType::Position>::Semantic);
	//	bool hasUV = vertex.Has(AttributeMap<AttributeType::UV>::Semantic);
	//	bool hasNormal = vertex.Has(AttributeMap<AttributeType::Normal>::Semantic);
	//	bool hasTangent = vertex.Has(AttributeMap<AttributeType::Tangent>::Semantic);

	//	RAY_MEXPECTS(!hasNormal && !hasTangent,
	//		"[IndexedTriangleList] Normals and tangents not supported for plane yet");

	//	vertex.BeginVertex();
	//	if (hasPos)
	//		vertex[AttributeMap<AttributeType::Position>::Semantic] = Float3{ -0.5f, -0.5f, 0.0f };
	//	if (hasUV)
	//		vertex[AttributeMap<AttributeType::UV>::Semantic] = uvs[0];

	//	vertex.BeginVertex();
	//	if (hasPos)
	//		vertex[AttributeMap<AttributeType::Position>::Semantic] = Float3{ 0.5f, -0.5f, 0.0f };
	//	if (hasUV)
	//		vertex[AttributeMap<AttributeType::UV>::Semantic] = uvs[1];

	//	vertex.BeginVertex();
	//	if (hasPos)
	//		vertex[AttributeMap<AttributeType::Position>::Semantic] = Float3{ 0.5f, 0.5f, 0.0f };
	//	if (hasUV)
	//		vertex[AttributeMap<AttributeType::UV>::Semantic] = uvs[2];

	//	vertex.BeginVertex();
	//	if (hasPos)
	//		vertex[AttributeMap<AttributeType::Position>::Semantic] = Float3{ -0.5f, 0.5f, 0.0f };
	//	if (hasUV)
	//		vertex[AttributeMap<AttributeType::UV>::Semantic] = uvs[3];

	//	std::vector<IndexBuffer::Type> indices{ 0, 1, 2, 2, 3, 0 };
	//	std::string tag = String::Serialize(
	//		"Plane#012230#{0}#{1}#{2}#{3}#{4}", hasPos, hasUV, hasNormal, hasTangent, charToRender);

	//	return Mesh::VertexData{ Codex::Resolve<VertexBuffer>(tag, vertex),
	//		Codex::Resolve<IndexBuffer>(tag, indices) };
	//}


	// TextRenderer::TextRenderer(Entity entity, Ref<FlatTextMaterial> material)
	//	: Component(entity), Renderer(material, false)
	//{
	//	char charToRender = 'j';

	//	// GetEntity().Emplace<Ray::Mesh>(GeneratePlane(material, charToRender));
	//	GetEntity().Emplace<Ray::Mesh>(Ray::Mesh::Plane(material));

	//	AddSampler2DUniform("samplerTextureAtlas", ShaderStage::Fragment,
	//		material->GetFont().GetSharedTextureAtlas());
	//}

	// void TextRenderer::Update()
	//{
	//	if (auto& tform = GetEntity().Get<Transform>(); tform.HasChanged())
	//	{
	//		if (GetEntity().HasParent())
	//			GetBufferUniform("PerObjectData")["Model"] =
	//				GetEntity().GetParent().Get<Transform>().AsMatrix() * tform.AsMatrix();
	//		else
	//			GetBufferUniform("PerObjectData")["Model"] = tform.AsMatrix();
	//	}
	//}

	// void TextRenderer::Render(const CommandBuffer& cmdBuff) const
	//{
	//	m_Material->CmdBind(cmdBuff);

	//	for (const auto& descSet : m_DescriptorSets)
	//		descSet.CmdBind(cmdBuff);

	//	GetEntity().Get<Mesh>().CmdBind(cmdBuff);
	//}
}  // namespace At0::Ray
