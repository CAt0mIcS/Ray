#pragma once

#include "../RBase.h"
#include "REntity.h"


namespace At0::Ray
{
	class RAY_EXPORT Scene
	{
	public:
		static Scene& Get()
		{
			static Scene instance;
			return instance;
		}

		virtual ~Scene();

		/**
		 * @returns A newly created, empty entity
		 */
		Entity& CreateEntity();

	protected:
		Scene();

	private:
		entt::registry m_Registry;

		// Heap allocate them to avoid Entity& from being invalid when the vector is resized
		std::vector<Scope<Entity>> m_Entities;
	};

}  // namespace At0::Ray
