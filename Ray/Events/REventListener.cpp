#include "REventListener.h"

#include "REngineEvents.h"
#include "RCustomEvents.h"
#include "RKeyboardEvents.h"
#include "RMouseEvents.h"

namespace At0::Ray
{
	template class RAY_EXPORT EventListener<WindowResizedEvent>;
	template class RAY_EXPORT EventListener<FramebufferResizedEvent>;
	template class RAY_EXPORT EventListener<WindowMovedEvent>;
	template class RAY_EXPORT EventListener<WindowClosedEvent>;

	template class RAY_EXPORT EventListener<CameraChangedEvent>;
	template class RAY_EXPORT EventListener<EntityCreatedEvent>;
	template class RAY_EXPORT EventListener<MaterialBecameDirtyEvent>;
	template class RAY_EXPORT EventListener<TransformChangedEvent>;

	template class RAY_EXPORT EventListener<KeyPressedEvent>;
	template class RAY_EXPORT EventListener<KeyReleasedEvent>;
	template class RAY_EXPORT EventListener<KeyRepeatedEvent>;
	template class RAY_EXPORT EventListener<CharEvent>;

	template class RAY_EXPORT EventListener<MouseButtonPressedEvent>;
	template class RAY_EXPORT EventListener<MouseButtonReleasedEvent>;
	template class RAY_EXPORT EventListener<MouseMovedEvent>;
	template class RAY_EXPORT EventListener<ScrollUpEvent>;
	template class RAY_EXPORT EventListener<ScrollDownEvent>;
	template class RAY_EXPORT EventListener<ScrollLeftEvent>;
	template class RAY_EXPORT EventListener<ScrollRightEvent>;
	template class RAY_EXPORT EventListener<HoverEnterEvent>;
	template class RAY_EXPORT EventListener<HoverLeaveEvent>;
}  // namespace At0::Ray