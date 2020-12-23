#pragma once

#include "RInternal.h"

#include <vector>


namespace At0::Ray::ECS
{
	class SparseSet
	{
	public:
		class SparseSetIterator
		{
		private:
			using Iterator = SparseSetIterator;

		public:
			SparseSetIterator(const std::vector<Entity>& packed, IndexType idx)
				: m_Packed{ packed }, m_Index{ idx } {}

			Iterator& operator++()
			{
				return ++m_Index, *this;
			}

			Iterator& operator--()
			{
				return --m_Index, *this;
			}

			bool operator==(const Iterator& other) const
			{
				return m_Index == other.m_Index;
			}

			bool operator!=(const Iterator& other) const
			{
				return !(*this == other);
			}

			const Entity* operator->() const
			{
				return &m_Packed[m_Index];
			}

			const Entity& operator*() const
			{
				return *operator->();
			}

		private:
			const std::vector<Entity> m_Packed;
			IndexType m_Index;
		};

	public:
		void Emplace(Entity e)
		{
			m_Sparse.emplace_back(m_LastIndex);
			m_Packed.emplace_back(e);

			++m_LastIndex;
		}

		void Remove(Entity e)
		{
			Entity temp = m_Packed[m_LastIndex - 1];
			m_Packed[m_Sparse[e]] = temp;
			m_Sparse[temp] = m_Sparse[e];

			--m_LastIndex;
		}

		IndexType Index(Entity e) const
		{
			return m_Sparse[e];
		}

		bool Contains(Entity e) const
		{
			return (e < m_Sparse.size());
		}
		
		SparseSetIterator begin() const
		{
			return SparseSetIterator{ m_Packed, 0 };
		}

		SparseSetIterator end() const
		{
			return SparseSetIterator{ m_Packed, m_Packed.size() };
		}

		IndexType Size() const
		{
			return (IndexType)m_Packed.size();
		}

	private:
		std::vector<Entity> m_Packed;
		std::vector<IndexType> m_Sparse;
		IndexType m_LastIndex = 0;
	};
}