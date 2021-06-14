#pragma once

#include "RNode.h"


namespace At0::Ray
{
	class RAY_EXPORT OutputNode : public Node
	{
	public:
		enum Connection
		{
			Input
		};

	public:
		OutputNode(std::string_view outputType, std::string_view outputName);

		std::string GetFunctionCalls(OutputID outputID) const override;

		std::string_view GetType() const { return m_OutputType; }
		std::string_view GetName() const { return m_OutputName; }

	private:
		void OnChildConnected(
			Ref<Node> childNode, OutputID childConnectionID, InputID connectionID) override;

	private:
		std::string m_OutputType;
		std::string m_OutputName;
	};
}  // namespace At0::Ray
