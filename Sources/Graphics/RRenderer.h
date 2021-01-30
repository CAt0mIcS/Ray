#pragma once

#include "../RBase.h"

#include <vector>


namespace At0::Ray
{
	class RenderStage;

	class Renderer
	{
	public:
		Renderer() = default;
		virtual ~Renderer() = default;

		/**
		 * Run when switching to this scene from another, use this method to create subrenderers.
		 */
		virtual void Start() = 0;

		/**
		 * Run when updating the renderer manager.
		 */
		virtual void Update() = 0;

		RenderStage* GetRenderStage(uint32_t index) const;

	protected:
		/**
		 * Adds a new render stage to the list of render stages
		 */
		void AddRenderStage(Scope<RenderStage>&& renderStage);

	private:
		bool m_Started = false;
		std::vector<Scope<RenderStage>> m_RenderStages;
	};
}  // namespace At0::Ray
