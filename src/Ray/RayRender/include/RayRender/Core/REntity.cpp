#include "REntity.h"

namespace At0::Ray
{
	Entity::Entity()
	{
		m_Entity = s_Registry.create();
	}

	Entity::~Entity()
	{
		s_Registry.destroy(m_Entity);
	}
}


