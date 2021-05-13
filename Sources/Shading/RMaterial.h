#pragma once

#include "../RBase.h"
#include "../Graphics/Core/RBindable.h"


namespace At0::Ray
{
	class GraphicsPipeline;

	class RAY_EXPORT Material : public Bindable
	{
	public:
		virtual ~Material() = default;

		const GraphicsPipeline& GetGraphicsPipeline() const { return *m_GraphicsPipeline; }
		GraphicsPipeline& GetGraphicsPipeline() { return *m_GraphicsPipeline; }

		/**
		 * Bind the graphics pipeline
		 */
		void CmdBind(const CommandBuffer& cmdBuff) const;

	protected:
		Material();

	protected:
		Ref<GraphicsPipeline> m_GraphicsPipeline;
	};

}  // namespace At0::Ray
