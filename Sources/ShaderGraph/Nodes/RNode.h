#pragma once

#include "../../RBase.h"
#include "../../Utils/RNonCopyable.h"


namespace At0::Ray
{
	class RAY_EXPORT Node : NonCopyable
	{
	public:
		using OutputID = uint32_t;
		using InputID = uint32_t;

		static constexpr uint32_t s_DefaultOutput = (uint32_t)-1;

	public:
		virtual ~Node() = default;

		/**
		 * Fills an input connection point of this node with another node, e.g.
		 * samplerNode.Connect(TextureNode, TextureNode::Texture, SamplerNode::Texture);
		 *
		 * @param node Output of node will be connected to input of this node at pointID
		 */
		void Connect(Ref<Node> childNode, OutputID childConnectionID, InputID connectionID);

		virtual std::string GetAttributes(uint32_t& inputLocation, uint32_t& outputLocation,
			OutputID outputID = s_DefaultOutput) const;
		virtual std::string GetFunctionCalls(OutputID outputID = s_DefaultOutput) const;

		/**
		 * Checks if a connection at the connection ID exists
		 */
		bool HasConnection(InputID connectionID) const;

	protected:
		Node() = default;

		/**
		 * Called when one of this node's inputs is connected to a child
		 */
		virtual void OnChildConnected(
			Ref<Node> childNode, OutputID childConnectionID, InputID connectionID);

		/**
		 * Called when a parent node connects to an output of this one
		 */
		virtual void OnParentConnected(
			Node* parentNode, InputID parentConnectionID, OutputID connectionID);

	protected:
		struct ChildNode
		{
			// child node
			Ref<Node> node;

			// Output id of child node
			OutputID connectionPoint{};
		};

		// Maps id of an input connection to child
		std::unordered_map<InputID, ChildNode> m_Children;

		// List of all required functions for this node
		std::vector<std::string> m_Functions;

		// Maps output connection id to function call, e.g.
		// splitNode.GetFunctionCalls(SplitNode::R) only gets function calls needed for output R
		struct FunctionCall
		{
			OutputID outputConnectionID{};
			std::string call;
		};
		std::vector<FunctionCall> m_FunctionCalls;
	};
}  // namespace At0::Ray
