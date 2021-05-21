#pragma once

#include "RTechnique.h"


namespace At0::Ray
{
	class Texture2D;

	class RAY_EXPORT Texture2DTechnique : public Technique
	{
	public:
		Texture2DTechnique(Ref<Texture2D> texture);

		void SetTexture(Ref<Texture2D> texture) { m_Texture = std::move(texture); }
		const Texture2D& GetTexture() const { return *m_Texture; }
		Texture2D& GetTexture() { return *m_Texture; }
		Ref<Texture2D> GetSharedTexture() const { return m_Texture; }

	private:
		Ref<Texture2D> m_Texture;
	};
}  // namespace At0::Ray
