#include "Rpch.h"
#include "RVector4Node.h"

#include "Ray/Utils/RString.h"


namespace At0::Ray
{
	std::string Vector4Node::GetFunctionCalls(OutputID outputID) const
	{
		const ChildNode& childX = m_Children.at(Vector4Node::R);
		const ChildNode& childY = m_Children.at(Vector4Node::G);
		const ChildNode& childZ = m_Children.at(Vector4Node::B);
		const ChildNode& childW = m_Children.at(Vector4Node::A);

		if (outputID == Vector4Node::Result)
		{
			return String::Serialize("vec4({0}, {1}, {2}, {3})",
				childX.node->GetFunctionCalls(childX.connectionPoint),
				childY.node->GetFunctionCalls(childY.connectionPoint),
				childZ.node->GetFunctionCalls(childZ.connectionPoint),
				childW.node->GetFunctionCalls(childW.connectionPoint));
		}

		return "";
	}
}  // namespace At0::Ray
