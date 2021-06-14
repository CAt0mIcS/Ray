#pragma once

#include "RNode.h"


namespace At0::Ray
{
	/**
	 * Specifies a buffer uniform input in the shader
	 */
	class RAY_EXPORT PropertyNode : public Node
	{
	public:
		enum Connection
		{
			Result
		};

	public:
		PropertyNode(std::string_view uBlockName, std::string_view uniformType,
			std::string_view uniformName);

		std::string GetFunctionCalls(OutputID outputID) const override;

	private:
		void OnParentConnected(
			Node* parentNode, InputID parentConnectionID, OutputID connectionID) override;

	private:
		std::string m_UniformBlockName;
		std::pair<std::string, std::string> m_Uniform;
	};
}  // namespace At0::Ray
