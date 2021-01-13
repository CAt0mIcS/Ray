#pragma once

#include "../RRBase.h"
#include "../Primitives/RDrawable.h"


namespace At0::Ray
{
	class RR_API Scene
	{
	public:
		using Iterator = typename std::vector<Drawable>::iterator;
		using ReverseIterator = typename std::vector<Drawable>::reverse_iterator;
		using ConstIterator = typename std::vector<Drawable>::const_iterator;
		using ConstReverseIterator = typename std::vector<Drawable>::const_reverse_iterator;

	public:
		Scene();
		~Scene();

		/// <summary>
		/// Submit a drawable to store in the scene which will be rendered
		/// when the scene is submitted to the Renderer.
		/// </summary>
		void Submit(Drawable&& drawable);

		Iterator				begin() { return m_Drawables.begin(); }
		Iterator				end() { return m_Drawables.end(); }
		ConstIterator			begin() const { return m_Drawables.cbegin(); }
		ConstIterator			end() const { return m_Drawables.cend(); }
		ReverseIterator			rbegin() { return m_Drawables.rbegin(); }
		ReverseIterator			rend() { return m_Drawables.rend(); }
		ConstReverseIterator	crbegin() const { return m_Drawables.crbegin(); }
		ConstReverseIterator	crend() const { return m_Drawables.crend(); }

	private:
		std::vector<Drawable> m_Drawables;
	};
}
