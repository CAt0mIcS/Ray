#include "Rpch.h"
#include "RTextRenderer.h"


namespace At0::Ray
{
	TextRenderer::TextRenderer(Entity entity, Ref<Material> material)
		: Component(entity), Renderer(std::move(material))
	{
	}
}  // namespace At0::Ray
