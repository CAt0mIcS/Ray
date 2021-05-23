#pragma once

#include "RNode.h"


namespace At0::Ray
{
	class Texture2D;

	class RAY_EXPORT Sampler2DNode : public Node
	{
	public:
		enum Connection
		{
			// Inputs
			Texture,
			UV,

			// Outputs
			Result
		};

	public:
		Sampler2DNode(std::string_view samplerName = "");

		std::string GetFunctionCalls(OutputID outputID) const override;

	private:
		void OnParentConnected(
			Node* parentNode, InputID parentConnectionID, OutputID connectionID) override;

	private:
		static uint32_t s_SamplerID;
		std::string m_Name;
	};
}  // namespace At0::Ray
