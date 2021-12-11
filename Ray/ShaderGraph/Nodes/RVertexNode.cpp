#include "Rpch.h"
#include "RVertexNode.h"

#include "Core/RDynamicVertex.h"


namespace At0::Ray
{
	std::string VertexNode::GetFunctionCalls(OutputID outputID) const
	{
		switch (outputID)
		{
		case VertexNode::Position: return AttributeMap<AttributeType::Position>::Semantic;
		case VertexNode::UV: return AttributeMap<AttributeType::UV>::Semantic;
		case VertexNode::Normal: return AttributeMap<AttributeType::Normal>::Semantic;
		case VertexNode::Tangent: return AttributeMap<AttributeType::Tangent>::Semantic;
		}

		Log::Warn(
			"[VertexNode] Invalid output connection ID {0}. No function call generated", outputID);

		return "";
	}

	void VertexNode::OnParentConnected(
		Node* parentNode, InputID parentConnectionID, OutputID connectionID)
	{
		switch (connectionID)
		{
		case VertexNode::Position:
			m_Attributes[AttributeMap<AttributeType::Position>::Semantic] =
				Node::AttributeData{ "in", AttributeMap<AttributeType::Position>::Type };
			return;
		case VertexNode::UV:
			m_Attributes[AttributeMap<AttributeType::UV>::Semantic] =
				Node::AttributeData{ "in", AttributeMap<AttributeType::UV>::Type };
			return;
		case VertexNode::Normal:
			m_Attributes[AttributeMap<AttributeType::Normal>::Semantic] =
				Node::AttributeData{ "in", AttributeMap<AttributeType::Normal>::Type };
			return;
		case VertexNode::Tangent:
			m_Attributes[AttributeMap<AttributeType::Tangent>::Semantic] =
				Node::AttributeData{ "in", AttributeMap<AttributeType::Tangent>::Type };
			return;
		}
		Log::Warn("[VertexNode] Invalid output connection ID {0}. No input attributes generated",
			connectionID);
	}
}  // namespace At0::Ray
