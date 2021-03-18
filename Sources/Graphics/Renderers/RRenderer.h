#pragma once

#include "../../RBase.h"
#include "../../Utils/RNonCopyable.h"
#include "../../Core/RTime.h"

#include "RSubRenderer.h"

#include <concepts>


namespace At0::Ray
{
	class RAY_EXPORT Renderer : NonCopyable
	{
	public:
		static Renderer& Get();

		/**
		 * Adds a subrenderer to the list of subrenderers
		 */
		template<typename T, typename... Args>
		SubRenderer& Emplace(Args&&... args) requires std::derived_from<T,
			SubRenderer>&& std::constructible_from<T, Args...>
		{
			return *m_SubRenderers.emplace_back(MakeScope<T>(std::forward<Args>(args)...));
		}

		/**
		 * Updates all subrenderers
		 */
		void Update(Delta ts);

		/**
		 * Binds all subrenderer resources
		 */
		void Bind(const CommandBuffer& cmdBuff) const;

	private:
		std::vector<Scope<SubRenderer>> m_SubRenderers;
	};
}  // namespace At0::Ray
