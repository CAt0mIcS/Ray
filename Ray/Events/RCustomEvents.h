#pragma once

#include "../Graphics/Pipelines/Shader/RShaderTypes.h"


namespace At0::Ray
{
	class Entity;

	class CameraChangedEvent
	{
	public:
		CameraChangedEvent() = default;
	};


	/**
	 * Gets dispatched to listeners when entity is added to the scene
	 */
	class EntityCreatedEvent
	{
	public:
		EntityCreatedEvent(Entity& entity) : m_Entity(entity) {}
		Entity& GetEntity() const { return m_Entity; }

	private:
		Entity& m_Entity;
	};


	struct MaterialBecameDirtyEvent
	{
		const std::string& dataPath;
		UniformType uType;
		VkImageLayout imageLayout = VK_IMAGE_LAYOUT_MAX_ENUM;
	};

	class Transform;
	struct TransformChangedEvent
	{
		const Transform& transform;
	};
}  // namespace At0::Ray