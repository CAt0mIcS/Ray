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
		Entity CreateEntity();

	protected:
		Scene();

	private:
		entt::registry m_Registry;
	};

}  // namespace At0::Ray
