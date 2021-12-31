#include "RTextRenderer.h"

#include "RMesh.h"
#include "RTransform.h"
#include "RMeshRenderer.h"
#include "Shading/RMaterial.h"

#include "Graphics/Text/RFont.h"
#include "Graphics/Pipelines/Shader/DataAccess/RBufferUniform.h"
#include "Graphics/Pipelines/RGraphicsPipeline.h"
#include "Graphics/Pipelines/Shader/DataAccess/RDescriptor.h"
#include "Graphics/Buffers/RIndexBuffer.h"
#include "Graphics/Buffers/RVertexBuffer.h"
#include "Graphics/RCodex.h"
#include "Core/RDynamicVertex.h"


namespace At0::Ray
{
	static Mesh::VertexData GeneratePlane(const Shader& shader, const Font& font, char charToRender)
	{
		// const Texture2DAtlas::Area& area = font.GetGlyph(charToRender).area;

		// std::array<Float2, 4> uvs = font.GetTextureAtlas().MapUV(area);

		// DynamicVertex vertex(shader);

		// bool hasPos = vertex.Has(AttributeMap<AttributeType::Position>::Semantic);
		// bool hasUV = vertex.Has(AttributeMap<AttributeType::UV>::Semantic);
		// bool hasNormal = vertex.Has(AttributeMap<AttributeType::Normal>::Semantic);
		// bool hasTangent = vertex.Has(AttributeMap<AttributeType::Tangent>::Semantic);

		// RAY_MEXPECTS(!hasNormal && !hasTangent,
		//	"[IndexedTriangleList] Normals and tangents not supported for plane yet");

		// vertex.BeginVertex();
		// if (hasPos)
		//	vertex[AttributeMap<AttributeType::Position>::Semantic] = Float3{ -0.5f, -0.5f, 0.0f };
		// if (hasUV)
		//	vertex[AttributeMap<AttributeType::UV>::Semantic] = uvs[0];

		// vertex.BeginVertex();
		// if (hasPos)
		//	vertex[AttributeMap<AttributeType::Position>::Semantic] = Float3{ 0.5f, -0.5f, 0.0f };
		// if (hasUV)
		//	vertex[AttributeMap<AttributeType::UV>::Semantic] = uvs[1];

		// vertex.BeginVertex();
		// if (hasPos)
		//	vertex[AttributeMap<AttributeType::Position>::Semantic] = Float3{ 0.5f, 0.5f, 0.0f };
		// if (hasUV)
		//	vertex[AttributeMap<AttributeType::UV>::Semantic] = uvs[2];

		// vertex.BeginVertex();
		// if (hasPos)
		//	vertex[AttributeMap<AttributeType::Position>::Semantic] = Float3{ -0.5f, 0.5f, 0.0f };
		// if (hasUV)
		//	vertex[AttributeMap<AttributeType::UV>::Semantic] = uvs[3];

		// std::vector<IndexBuffer::Type> indices{ 0, 1, 2, 2, 3, 0 };
		// std::string tag = String::Serialize(
		//	"Plane#012230#{0}#{1}#{2}#{3}#{4}", hasPos, hasUV, hasNormal, hasTangent, charToRender);

		// return Mesh::VertexData{ Codex::Resolve<VertexBuffer>(tag, vertex),
		//	Codex::Resolve<IndexBuffer>(tag, indices), nullptr, {}, RAY_DEBUG_FLAG(tag) };
	}


	TextRenderer::TextRenderer(
		Entity entity, Ref<Material> material, const Font& font, std::string_view toRender)
		: Component(entity), Renderer(std::move(material)),
		  m_Mesh(GetEntity(), Mesh::Plane(m_Material))
	{
		// GetEntity().Emplace<Ray::Mesh>(
		//	GeneratePlane(m_Material->GetGraphicsPipeline().GetShader(), font, charToRender));
		// GetEntity().Emplace<MeshRenderer>(m_Material);
	}

	void TextRenderer::Update()
	{
		if (auto& tform = GetEntity().Get<Transform>(); tform.HasChanged())
		{
			// RAY_TODO: Fix parent transforms
			// if (GetEntity().HasParent())
			//	GetBufferUniform("PerObjectData")["Model"] =
			//		GetEntity().GetParent().Get<Transform>().AsMatrix() * tform.AsMatrix();
			// else
			// GetBufferUniform("PerObjectData")["Model"] = tform.AsMatrix();
		}
	}

	void TextRenderer::Render(const CommandBuffer& cmdBuff) const
	{
		// m_Material->CmdBind(cmdBuff);

		// for (const auto& descSet : m_DescriptorSets)
		//	descSet.CmdBind(cmdBuff);
	}
}  // namespace At0::Ray
