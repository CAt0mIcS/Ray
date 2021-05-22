#pragma once

#include "../../RBase.h"
#include "../../Utils/RNonCopyable.h"


namespace At0::Ray
{
	class RAY_EXPORT Node : NonCopyable
	{
	public:
		virtual ~Node() = default;

		/**
		 * Fills an input connection point of this node with another node, e.g.
		 * samplerNode.Connect(TextureNode, TextureNode::Texture, SamplerNode::Texture);
		 *
		 * @param node Output of node will be connected to input of this node at pointID
		 */
		void Connect(Scope<Node> childNode, uint32_t childPointID, uint32_t pointID);

	protected:
		Node() = default;

	protected:
		struct ChildNode
		{
			// child node
			Scope<Node> node;

			// Output id of child node
			uint32_t connectionPoint{};
		};

		// Maps id of an input connection to child
		std::unordered_map<uint32_t, ChildNode> m_Children;
	};
}  // namespace At0::Ray
