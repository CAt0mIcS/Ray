#pragma once

#include "../RRBase.h"
#include "../Core/REntity.h"


namespace At0::Ray
{
	class RR_API Drawable
	{
	public:
		template<typename Component, typename... Args>
		decltype(auto) AddComponent(Args&&... args)
		{
			m_Entity.Emplace<Component>(std::forward<Args>(args)...);
		}

		template<typename Component>
		void RemoveComponent()
		{
			m_Entity.Remove<Component>();
		}

		template<typename Component>
		bool HasComponent() const
		{
			return m_Entity.Has<Component>();
		}

		virtual ~Drawable() = default;
	protected:


	private:
		Entity m_Entity;
	};
}