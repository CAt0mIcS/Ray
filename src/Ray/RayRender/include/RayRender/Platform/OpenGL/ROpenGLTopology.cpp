#include "../../RRpch.h"

#include "ROpenGLTopology.h"

#include <RayDebug/RAssert.h>
#include <glad/glad.h>

namespace At0::Ray
{
	OpenGLTopology::OpenGLTopology(Type type)
		: m_Type(type)
	{

	}

	void OpenGLTopology::Bind()
	{

	}

	int OpenGLTopology::Get() const
	{
		switch (m_Type)
		{
		case Type::TriangleList:	return GL_TRIANGLES;
		}

		RAY_ASSERT(false, "[OpenGLTopology::Get] Invalid Topology::Type ({0}) selected.", (uint32_t)m_Type);
		return 0;
	}
}
