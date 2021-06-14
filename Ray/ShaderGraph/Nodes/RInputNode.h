#pragma once

#include "RNode.h"


namespace At0::Ray
{
	class OutputNode;

	class RAY_EXPORT InputNode : public Node
	{
	public:
		enum Connection
		{
			Result
		};

	public:
		InputNode(std::string_view inputType, std::string_view inputName);
		InputNode(const OutputNode& outputNode, std::string_view inputName);

		std::string GetFunctionCalls(OutputID outputID) const override;

	private:
		void OnParentConnected(
			Node* parentNode, InputID parentConnectionID, OutputID connectionID) override;

	private:
		std::string m_InputType;
		std::string m_InputName;
	};
}  // namespace At0::Ray
