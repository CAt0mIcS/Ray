#pragma once

#include "RStorage.h"

#include <tuple>
#include <type_traits>


namespace At0::Ray::ECS
{
	template<typename... Component>
	class ComponentView
	{
	private:
		template<typename Comp>
		using PoolType = std::conditional_t<std::is_const_v<Comp>, const Storage<std::remove_const_t<Comp>>, Storage<Comp>>;

	public:
		class ViewIterator
		{
		public:
			ViewIterator(const SparseSet& view, SparseSet::SparseSetIterator it)
				: m_View{ view }, m_SparseSetIterator{ it } {}

			ViewIterator& operator++()
			{
				++m_SparseSetIterator;
				return *this;
			}

			ViewIterator& operator--()
			{
				--m_SparseSetIterator;
				return *this;
			}

			bool operator==(const ViewIterator& other) const
			{
				return m_SparseSetIterator == other.m_SparseSetIterator;
			}

			bool operator!=(const ViewIterator& other) const
			{
				return !(*this == other);
			}

			const Entity* operator->() const
			{
				return m_SparseSetIterator.operator->();
			}

			const Entity& operator*() const
			{
				return *operator->();
			}

		private:
			const SparseSet& m_View;
			SparseSet::SparseSetIterator m_SparseSetIterator;
		};

	public:
		ComponentView(Storage<Component>&... component)
			: m_Components{ &component... }
		{

		}

		ViewIterator begin()
		{
			const SparseSet& view = Candidate();
			return ViewIterator{ view, view.begin() };
		}

		ViewIterator end()
		{
			const SparseSet& view = Candidate();
			return ViewIterator{ view, view.end() };
		}

	private:
		const SparseSet& Candidate() const
		{
			return *std::min({ (const SparseSet*)(std::get<PoolType<Component>*>(m_Components))... }, [](const SparseSet* lhs, const SparseSet* rhs)
				{
					return lhs->Size() < rhs->Size();
				}
			);
		}

	private:
		std::tuple<PoolType<Component>*...> m_Components;
	};
}