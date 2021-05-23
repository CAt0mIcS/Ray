#pragma once

#include "RNode.h"


namespace At0::Ray
{
	class RAY_EXPORT VertexNode : public Node
	{
	public:
		enum Connection
		{
			// Output
			ScreenSpacePosition
		};

	public:
		std::string GetFunctionCalls(OutputID outputID) const override;

	private:
		void OnParentConnected(
			Node* parentNode, InputID parentConnectionID, OutputID connectionID) override;
	};
}  // namespace At0::Ray
