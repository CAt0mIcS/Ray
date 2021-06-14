#include "Rpch.h"
#include "RTexture2DNode.h"


namespace At0::Ray
{
	Texture2DNode::Texture2DNode(Ref<Texture2D> texture) : m_Texture(std::move(texture)) {}
}  // namespace At0::Ray