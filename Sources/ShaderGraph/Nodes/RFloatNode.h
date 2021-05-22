#pragma once

#include "RNode.h"


namespace At0::Ray
{
	class RAY_EXPORT FloatNode : public Node
	{
	public:
		enum Connection
		{
			// Output
			Result
		};

	public:
		FloatNode(float value);

		float GetFloat() const { return m_Value; }
		void SetFloat(float value) { m_Value = value; }

	private:
		float m_Value;
	};
}  // namespace At0::Ray
