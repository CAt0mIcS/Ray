// #include "RTextComponent.h"
//
// #include "../RMesh.h"
// #include "../RHierachyComponent.h"
//
// #include "Core/RDynamicVertex.h"
// #include "Core/RMath.h"
//
// #include "Shading/RMaterial.h"
// #include "Graphics/Pipelines/RGraphicsPipeline.h"
// #include "Graphics/Buffers/RIndexBuffer.h"
// #include "Graphics/Buffers/RVertexBuffer.h"
// #include "Graphics/Pipelines/Shader/RShader.h"
// #include "Graphics/Text/RFont.h"
//
// #include "Devices/RWindow.h"
// #include "Scene/RScene.h"
//
//
// namespace At0::Ray
//{
//	static Mesh::Data GetPlane(Ref<Material> material, float xPos, float yPos, float w, float h)
//	{
//		DynamicVertex vertex(material->GetGraphicsPipeline().GetShader());
//		bool hasUV = vertex.Has(AttributeMap<AttributeType::UV>::Semantic);
//
//		vertex.BeginVertex();
//		vertex[AttributeMap<AttributeType::Position>::Semantic] = Float3{ xPos + w, yPos + h, 0.f };
//		if (hasUV)
//			vertex[AttributeMap<AttributeType::UV>::Semantic] = Float2{ 1.f, 0.f };
//
//		vertex.BeginVertex();
//		vertex[AttributeMap<AttributeType::Position>::Semantic] = Float3{ xPos, yPos + h, 0.f };
//		if (hasUV)
//			vertex[AttributeMap<AttributeType::UV>::Semantic] = Float2{ 0.f, 0.f };
//
//		vertex.BeginVertex();
//		vertex[AttributeMap<AttributeType::Position>::Semantic] = Float3{ xPos, yPos, 0.f };
//		if (hasUV)
//			vertex[AttributeMap<AttributeType::UV>::Semantic] = Float2{ 0.f, 1.f };
//
//		vertex.BeginVertex();
//		vertex[AttributeMap<AttributeType::Position>::Semantic] = Float3{ xPos + w, yPos, 0.f };
//		if (hasUV)
//			vertex[AttributeMap<AttributeType::UV>::Semantic] = Float2{ 1.f, 1.f };
//
//		std::vector<IndexBuffer::Type> indices{ 0, 1, 2, 2, 3, 0 };
//
//		return { MakeRef<VertexBuffer>("VtxText", vertex), MakeRef<IndexBuffer>("IdxText", indices),
//			std::move(material) };
//	}
//
//
//	TextComponent::TextComponent(
//		Entity entity, const Window& window, std::string text, Ref<Font> font)
//		: Component(entity), m_Text(std::move(text)), m_Font(std::move(font)), m_Window(&window)
//	{
//		if (!GetEntity().Has<HierachyComponent>())
//			GetEntity().Emplace<HierachyComponent>();
//
//		LoadMeshes();
//	}
//
//	void TextComponent::SetText(std::string text)
//	{
//		m_Text = std::move(text);
//		for (Entity child : GetEntity().GetChildren())
//			child.Destroy();
//		GetEntity().Get<HierachyComponent>().GetChildren().clear();
//
//		LoadMeshes();
//	}
//
//	void TextComponent::LoadMeshes()
//	{
//		auto pipeline = GraphicsPipeline::Builder(
//			Graphics::Get().GetRenderPass(), Graphics::Get().GetPipelineCache())
//							.SetShader(Shader::FromSourceFile({ "Resources/Shaders/Flat_Text.vert",
//								"Resources/Shaders/Flat_Text.frag" }))
//							.SetCullMode(VK_CULL_MODE_NONE)
//							.Acquire();
//
//		float x{};
//		float y{};
//		float scale = 1.f;
//		for (uint8_t c : m_Text)
//		{
//			const Font::Glyph& glyph = m_Font->GetGlyph(c);
//
//			Ref<Material> textMaterial;
//
//			if (glyph.texture)
//				textMaterial =
//					Material::Builder(pipeline).Set("samplerText", glyph.texture).Build();
//			else
//			{
//				x += ScreenSpaceToNDCSpaceX(
//						 glyph.advance * scale, m_Window->GetFramebufferSize().x) +
//					 1.f;
//				continue;
//			}
//
//			UInt2 windowSize = m_Window->GetFramebufferSize();
//
//			// Float2 ndcBearing{ (Float2)glyph.bearing / (Float2)windowSize };
//			// Float2 ndcSize{ (Float2)glyph.size / (Float2)windowSize };
//
//			Float2 ndcBearing =
//				ScreenSpaceToNDCSpace(glyph.bearing, m_Window->GetFramebufferSize()) +
//				Float2{ 1.f, 0.f };
//			Float2 ndcSize = ScreenSpaceToNDCSpace(glyph.size, m_Window->GetFramebufferSize()) +
//							 Float2{ 1.f, 0.f };
//
//			float xPos = x + ndcBearing.x * scale;
//			float yPos = y - (ndcSize.y - ndcBearing.y) * scale;
//
//			float w = ndcSize.x * scale;
//			float h = ndcSize.y * scale;
//
//			Entity entity = Scene::Get().CreateEntity();
//			entity.Emplace<Mesh>(GetPlane(textMaterial, xPos, yPos, w, h));
//			entity.Emplace<HierachyComponent>().SetParent(GetEntity());
//			GetEntity().Get<HierachyComponent>().AddChild(entity);
//			// auto& tform = m_TextEntity.Get<Transform>();
//			// tform.SetRotation({ 3 * Math::PI<> / 2.f, Math::PI<>, 0.f });
//			// tform.SetScale({ w, 1.f, h });
//			// tform.SetTranslation({ xPos, yPos, 0.f });
//
//			x += ScreenSpaceToNDCSpaceX(glyph.advance * scale, m_Window->GetFramebufferSize().x) +
//				 1.f;
//			// x += glyph.advance * scale / windowSize.x;
//		}
//	}
// }  // namespace At0::Ray
