#pragma once

#include "../Control.h"


namespace GUI
{
	class TextBox;

	class TextCursor : public Control
	{
	public:
		TextCursor(TextBox* parent);

		virtual bool Render() override;

		virtual bool OnEvent(Event& e) override;

		virtual std::optional<std::pair<Util::NPoint, Util::NSize>> CalculateLayout(const Util::NPoint& parentPos, const Util::NSize& parentSize) override;

		void SetPositionInText(const unsigned int posInText) { m_PosInText = posInText; }

	private:
		unsigned int m_PosInText;
		TextBox* m_TxtParent;
		bool m_IsFocused;
	};
}



