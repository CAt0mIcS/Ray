#include "Rpch.h"
#include "RMultiplyNode.h"

#include "Utils/RString.h"


namespace At0::Ray
{
	std::string MultiplyNode::GetFunctionCalls(OutputID outputID) const
	{
		const ChildNode& childLeft = m_Children.at(MultiplyNode::Left);
		const ChildNode& childRight = m_Children.at(MultiplyNode::Right);

		if (outputID == MultiplyNode::Result)
			return String::Serialize("Ray_Multiply({0}, {1})",
				childLeft.node->GetFunctionCalls(childLeft.connectionPoint),
				childRight.node->GetFunctionCalls(childRight.connectionPoint));

		return "";
	}

	void MultiplyNode::OnParentConnected(
		Node* parentNode, InputID parentConnectionID, OutputID connectionID)
	{
		// clang-format off
		static constexpr const char* typeCombinations[][3] = 
		{ 
			{ 
				"float",    // Return type
				"float",    // Left input type
				"float"     //Right input type
			},
			{ 
				"vec2",
				"vec2",
				"vec2"
			},
			{ 
				"vec3",
				"vec3",
				"vec3"
			},
			{ 
				"vec4",
				"vec4",
				"vec4"
			},
			{ 
				"mat3",
				"mat3",
				"mat3"
			},
			{ 
				"mat4",
				"mat4",
				"mat4"
			},
			{ 
				"vec3", 
				"mat3", 
				"vec3"
			},
			{ 
				"vec4", 
				"mat4", 
				"vec4"
			},
		};
		// clang-format on

		for (uint32_t i = 0; i < std::size(typeCombinations); ++i)
		{
			m_Functions.emplace_back(
				String::Serialize("{0} Ray_Multiply({1} left, {2} right) { return left * right; }",
					typeCombinations[i][0], typeCombinations[i][1], typeCombinations[i][2]));
		}
	}
}  // namespace At0::Ray
