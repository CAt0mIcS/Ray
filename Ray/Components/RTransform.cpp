﻿#include "RTransform.h"
#include "RHierachyComponent.h"

#include "Events/REventListener.h"


namespace At0::Ray
{
	Transform& Transform::SetTranslation(Float3 translation)
	{
		m_Translation = translation;
		m_Changed = true;
		return *this;
	}

	Transform& Transform::SetRotation(Float3 rotation)
	{
		m_Rotation = rotation;
		m_Changed = true;
		return *this;
	}

	Transform& Transform::SetScale(Float3 scale)
	{
		m_Scale = scale;
		m_Changed = true;
		return *this;
	}

	Transform& Transform::Translate(Float3 translation)
	{
		m_Translation += translation;
		m_Changed = true;
		return *this;
	}

	Transform& Transform::Rotate(Float3 rotation)
	{
		m_Rotation += rotation;
		m_Changed = true;
		return *this;
	}

	Transform& Transform::Scale(Float3 scale)
	{
		m_Scale += scale;
		m_Changed = true;
		return *this;
	}

	void Transform::UpdateMatrix()
	{
		if (m_Changed)
		{
			const float c3 = cos(m_Rotation.z);
			const float s3 = sin(m_Rotation.z);
			const float c2 = cos(m_Rotation.x);
			const float s2 = sin(m_Rotation.x);
			const float c1 = cos(m_Rotation.y);
			const float s1 = sin(m_Rotation.y);
			// clang-format off
			 m_CachedMatrix = Matrix{
				{
					m_Scale.x * (c1 * c3 + s1 * s2 * s3),
					m_Scale.x * (c2 * s3),
					m_Scale.x * (c1 * s2 * s3 - c3 * s1),
					0.0f,
				},
				{
					m_Scale.y * (c3 * s1 * s2 - c1 * s3),
					m_Scale.y * (c2 * c3),
					m_Scale.y * (c1 * c3 * s2 + s1 * s3),
					0.0f,
				},
				{
					m_Scale.z * (c2 * s1),
					m_Scale.z * (-s2),
					m_Scale.z * (c1 * c2),
					0.0f,
				},
				{ m_Translation, 1.0f } };
			// clang-format on

			m_Changed = false;
			m_HasChanged = true;

			// TODO: Should we do this in e.g. SetTranslation or leave it here?
			InvokeEvent();
		}
		else
			m_HasChanged = false;
	}

	bool Transform::HasChanged() const
	{
		bool parentChanged = false;
		// RAY_TODO: Multithreaded transform recalculations in scene: the parent stuff fails
		if (auto hierachy = GetEntity().TryGet<HierachyComponent>();
			hierachy && hierachy->GetParent().Valid())
			parentChanged = hierachy->GetParent().Get<Transform>().HasChanged();

		return m_HasChanged || parentChanged;
	}

	Transform::Transform(Entity entity, Float3 translation, Float3 rotation, Float3 scale)
		: Component(entity), m_Translation{ translation }, m_Rotation{ rotation }, m_Scale{ scale }
	{
	}

	Transform::Transform(Entity entity, Float3 translation, Float3 rotation)
		: Component(entity), m_Translation{ translation }, m_Rotation{ rotation }
	{
	}

	Transform::Transform(Entity entity, Float3 translation)
		: Component(entity), m_Translation{ translation }, m_Rotation{ 0.0f, 0.0f, 0.0f }
	{
	}

	Transform::Transform(Entity entity) : Component(entity), m_Rotation{ 0.0f, 0.0f, 0.0f } {}

	const Matrix& Transform::AsMatrix()
	{
		//#ifndef NDEBUG
		if (m_Changed)
		{
			UpdateMatrix();
			// Log::Error("[Transform] Matrix should've already been recalculated");
		}
		//#endif
		return m_CachedMatrix;
	}

	Matrix3 Transform::GetNormalMatrix() const
	{
		const float c3 = cos(m_Rotation.z);
		const float s3 = sin(m_Rotation.z);
		const float c2 = cos(m_Rotation.x);
		const float s2 = sin(m_Rotation.x);
		const float c1 = cos(m_Rotation.y);
		const float s1 = sin(m_Rotation.y);
		// clang-format off
		return Matrix3{
			{
				m_Scale.x * (c1 * c3 + s1 * s2 * s3),
				m_Scale.x * (c2 * s3),
				m_Scale.x * (c1 * s2 * s3 - c3 * s1),
			},
			{
				m_Scale.y * (c3 * s1 * s2 - c1 * s3),
				m_Scale.y * (c2 * c3),
				m_Scale.y * (c1 * c3 * s2 + s1 * s3),
			},
			{
				m_Scale.z * (c2 * s1),
				m_Scale.z * (-s2),
				m_Scale.z * (c1 * c2),
			}
			// clang-format on
		};
	}

	void Transform::InvokeEvent()
	{
		TransformChangedEvent e{ *this };
		for (const auto listener : EventDispatcher<TransformChangedEvent>::Get())
			listener->OnEvent(e);
	}
}  // namespace At0::Ray
