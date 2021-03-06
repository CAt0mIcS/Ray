#pragma once

#include "RNode.h"


namespace At0::Ray
{
	class Texture2D;

	class RAY_EXPORT Texture2DNode : public Node
	{
	public:
		enum Connection
		{
			Output
		};

	public:
		Texture2DNode(Ref<Texture2D> texture);

		Ref<Texture2D> GetSharedTexture() const { return m_Texture; }

	private:
		Ref<Texture2D> m_Texture;
	};
}  // namespace At0::Ray
