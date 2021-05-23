#pragma once

#include "RNode.h"


namespace At0::Ray
{
	class RAY_EXPORT MultiplyNode : public Node
	{
	public:
		enum Connection
		{
			// Inputs
			Left,
			Right,

			// Output
			Result
		};

	public:
		std::string GetFunctionCalls(OutputID outputID) const override;

	private:
		virtual void OnParentConnected(
			Node* parentNode, InputID parentConnectionID, OutputID connectionID);
	};
}  // namespace At0::Ray
