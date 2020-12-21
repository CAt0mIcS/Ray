#pragma once

#include "RInternal.h"

#include <vector>


namespace At0::Ray::ECS
{
	class SparseSet
	{
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

		uint32_t Index(Entity e) const
		{
			return m_Sparse[e];
		}

		bool Contains(Entity e) const
		{
			return (e < m_Sparse.size());
		}

	private:
		std::vector<Entity> m_Packed;
		std::vector<uint32_t> m_Sparse;
		uint32_t m_LastIndex = 0;
	};
}