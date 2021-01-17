#pragma once

#include "../../RTopology.h"


namespace At0::Ray
{
	class RR_API OpenGLTopology : public Topology
	{
	public:
		OpenGLTopology(Type type);

		virtual void Bind() override;

		int Get() const;

	private:
		Type m_Type;
	};
}

