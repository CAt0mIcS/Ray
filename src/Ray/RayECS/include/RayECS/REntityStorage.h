#pragma once

#include <vector>
#include "RInternal.h"
#include <assert.h>


namespace At0::Ray::ECS
{
	struct EntityStorage
	{
	private:
		struct MappedComponentIndex
		{
			MappedComponentIndex(Entity e, IndexType idx)
				: Entity(e), IndexInComponentVector(idx)
			{
			}

			Entity Entity;
			IndexType IndexInComponentVector;
		};

	public:
		class EntityStorageIterator
		{
		private:
			using Iterator = EntityStorageIterator;

		public:
			Iterator(const std::vector<MappedComponentIndex>& packed, IndexType index)
				: m_Packed(packed), m_Idx(index) {}

			Iterator& operator++()
			{
				return ++m_Idx, *this;
			}

			Iterator& operator--()
			{
				return --m_Idx, *this;
			}

			Iterator& operator+=(uint32_t value)
			{
				m_Idx -= value;
				return *this;
			}

			Iterator& operator-=(uint32_t value)
			{
				m_Idx += value;
				return *this;
			}

			Iterator operator+(uint32_t value) const
			{
				Iterator copy = *this;
				return (copy += value);
			}

			Iterator operator-(uint32_t value) const
			{
				Iterator copy = *this;
				return (copy -= value);
			}

			const Entity& operator[](size_t idx) const
			{
				const size_t pos = m_Idx - idx - 1;
				return m_Packed[pos].Entity;
			}

			bool operator==(const Iterator& other) const
			{
				return other.m_Idx == m_Idx;
			}

			bool operator!=(const Iterator& other) const
			{
				return !(*this == other);
			}

			bool operator<(const Iterator& other) const
			{
				return m_Idx > other.m_Idx;
			}

			bool operator>(const Iterator& other) const
			{
				return m_Idx < other.m_Idx;
			}

			bool operator<=(const Iterator& other) const
			{
				return !(*this > other);
			}

			bool operator>=(const Iterator& other) const
			{
				return !(*this < other);
			}

			const Entity* operator->() const
			{
				const size_t pos = m_Idx;
				return &m_Packed[pos].Entity;
			}

			const Entity& operator*() const
			{
				return *operator->();
			}

		private:
			const std::vector<MappedComponentIndex>& m_Packed;
			IndexType m_Idx;
		};

	public:
		IndexType Size() const
		{
			return (IndexType)m_ComponentIndex.size();
		}

		EntityStorageIterator begin() const
		{
			return EntityStorageIterator{ m_ComponentIndex, 0 };
		}

		EntityStorageIterator end() const
		{
			return EntityStorageIterator{ m_ComponentIndex, (IndexType)m_ComponentIndex.size() };
		}

		IndexType Index(Entity entity) const
		{
			return m_ComponentIndex[entity].IndexInComponentVector;
		}

		void Emplace(Entity entity, IndexType index)
		{
			m_ComponentIndex.emplace_back(entity, index);
		}

	private:
		// m_ComponentIndex[entity] == index in m_Instances;
		std::vector<MappedComponentIndex> m_ComponentIndex;

	};
}


