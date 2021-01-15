#include "RDrawable.h"

#include "../Core/RScene.h"

#include <RayUtil/RComponents.h>
#include <RayDebug/RAssert.h>


namespace At0::Ray
{
	Drawable::Drawable(Entity e)
		: m_Entity(e)
	{
	}

	uint32_t Drawable::GetIndexBufferCount() const
	{
		RAY_MEXPECTS(m_IndexBufferCount != 0, "[Drawable::GetIndexBufferCount] Index Buffer count not set.");
		return m_IndexBufferCount;
	}
}


