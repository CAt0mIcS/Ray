#include "pch.h"
#include "TextBox.h"

#include "GUI/Graphics/Renderer.h"
#include "GUI/Graphics/TextRenderer.h"

#include "GUI/Events/MouseEvent.h"
#include "GUI/Events/KeyboardEvent.h"
#include "GUI/Events/ApplicationEvent.h"

#include "Util/Debug/Timer.h"

/**
* QUESTION:
*	Should I be designing something like this GUI library only for this project or should I make it usable in another one?
*	Should I write class declarations (class Node;) in header files instead of including them
*/



namespace GUI
{
	TextBox::TextBox(Control* parent)
		: Control(parent), m_Text{}, m_CurrentlySelecting(true), m_IsMultiline(false), m_Caret(this)
	{
		
	}

	bool TextBox::Render()
	{
		if (this->IsInWindow())
		{
			for (auto* child : m_Children)
			{
				child->Render();
			}

			const float max = std::min(GetSize().width, GetSize().height);
			Renderer::Get().RenderRoundedRect(GetPos(), GetSize(), GetColor(), max / 5.0f, max / 5.0f);
			RenderText();
			RenderCaret();

			return true;
		}

		return false;
	}

	bool TextBox::OnEvent(Event& e)
	{
		if (e.GetType() == EventType::MouseButtonPressedEvent)
		{
			return OnMouseButtonPressed((MouseButtonPressedEvent&)e);
		}
		else if (e.GetType() == EventType::MouseButtonReleasedEvent)
		{
			return OnMouseButtonReleased((MouseButtonReleasedEvent&)e);
		}
		else if (e.GetType() == EventType::SetCursorEvent)
		{
			return OnSetCursor((SetCursorEvent&)e);
		}
		
		return false;
	}

	void TextBox::RenderCaret()
	{
		auto caretRect = m_Caret.GetCaretRect();

		float xOffset = m_Size.width / 30.0f;
		caretRect.left += (m_Pos.x + xOffset);

		float yOffset;
		if (m_IsMultiline)
		{
			yOffset = m_Size.height / 10.0f;
		}
		else
		{
			yOffset = m_Size.height / 2.0f - m_Text.fontSize / 2.0f;
		}

		caretRect.top += m_Pos.y + yOffset;
		caretRect.right = caretRect.left + m_Caret.GetCaretThickness();
		caretRect.bottom += caretRect.top;

		Renderer::Get().RenderRect(caretRect, { 255, 255, 255 });
	}

	void TextBox::RenderText()
	{
		float xOffset = m_Size.width / 30.0f;
		float yOffset;
		if (m_IsMultiline)
		{
			yOffset = m_Size.height / 10.0f;
		}
		else
		{
			yOffset = m_Size.height / 2.0f - m_Text.fontSize / 2.0f;
		}

		m_Text.pos = Util::NPoint{ m_Pos.x + xOffset, m_Pos.y + yOffset };
		m_Text.size = { m_Size.width - xOffset, m_Size.height - yOffset };

		TextRenderer::Get().RenderText(m_Text);
	}

	std::optional<std::pair<Util::NPoint, Util::NSize>> TextBox::CalculateLayout(const Util::NPoint& parentPos, const Util::NSize& parentSize)
	{
		return { {{}, {}} };
	}

	bool TextBox::OnSetCursor(SetCursorEvent& e)
	{
		/**
		* Mouse is already guaranteed to be on the control, we don't need to check here
		* Default cursor is automatically restored when exiting the control
		*/
		SetCursor(LoadCursor(NULL, IDC_IBEAM));
		return true;
	}

	bool TextBox::OnMouseButtonPressed(MouseButtonPressedEvent& e)
	{
		if (e.GetButton() == MouseButton::Left)
		{
			m_CurrentlySelecting = true;
			
			bool extendSelection = false;
			BOOL isTrailingHit;
			BOOL isInside;
			
			auto caretMetrics = TextRenderer::Get().HitTestPoint(m_Text, &isTrailingHit, &isInside);

			m_Caret.SetSelection(
				isTrailingHit ? Caret::SetSelectionMode::AbsoluteTrailing : Caret::SetSelectionMode::AbsoluteLeading,
				caretMetrics.textPosition,
				extendSelection
			);

 			return true;
		}
		return false;
	}

	bool TextBox::OnMouseButtonReleased(MouseButtonReleasedEvent& e)
	{
		if (e.GetButton() == MouseButton::Left)
		{
			m_CurrentlySelecting = false;
			return true;
		}
		return false;
	}


	/******************
	*****  Caret  *****
	*******************/
	TextBox::Caret::Caret(TextBox* parent)
		: m_Parent(parent), m_CaretPos(0), m_CaretPosOffset(0), m_CaretAnchor(0), m_CaretFormat{}
	{

	}

	void TextBox::Caret::SetSelection(SetSelectionMode moveMode, unsigned int advance, bool extendSelection, bool updateCaretFormat)
	{
		unsigned int absolutePosition = m_CaretPos + m_CaretPosOffset;
		unsigned int oldAbsolutePosition = absolutePosition;
		unsigned int oldCaretAnchor = m_CaretAnchor;

		switch (moveMode)
		{
		case SetSelectionMode::Left:
		{
			break;
		}
		case SetSelectionMode::Right:
		{
			break;
		}
		case SetSelectionMode::LeftChar:
		{
			break;
		}
		case SetSelectionMode::RightChar:
		{
			break;
		}
		case SetSelectionMode::Up:
		case SetSelectionMode::Down:
		{
			break;
		}
		case SetSelectionMode::LeftWord:
		case SetSelectionMode::RightWord:
		{
			break;
		}
		case SetSelectionMode::AbsoluteLeading:
		{
			m_CaretPos = advance;
			m_CaretPosOffset = 0;
			break;
		}
		case SetSelectionMode::AbsoluteTrailing:
		{
			m_CaretPos = advance;
			AlignCaretToNearestCluster(true);
			break;
		}
		}

		absolutePosition = m_CaretPos + m_CaretPosOffset;
		if (!extendSelection)
			m_CaretAnchor = absolutePosition;

		bool caretMoved = (absolutePosition != oldAbsolutePosition)
			|| (m_CaretAnchor != oldCaretAnchor);

		if (caretMoved)
		{
			if (updateCaretFormat)
			{
				UpdateCaretFormatting();
			}
			Renderer::Get().BeginDraw();
			m_Parent->Render();
			Renderer::Get().EndDraw();
		}
	}

	void TextBox::Caret::AlignCaretToNearestCluster(bool isTrailingHit, bool skipZeroWidth)
	{
		float caretX, caretY;

		auto hitTestMetrics = TextRenderer::Get().HitTestTextPosition(
			m_Parent->GetText(),
			m_CaretPos,
			false,
			&caretX,
			&caretY
		);

		m_CaretPos = hitTestMetrics.textPosition;
		m_CaretPosOffset = (isTrailingHit) ? hitTestMetrics.length : 0;

		if (skipZeroWidth && hitTestMetrics.width == 0)
		{
			m_CaretPos += m_CaretPosOffset;
			m_CaretPosOffset = 0;
		}
	}

	void TextBox::Caret::UpdateCaretFormatting()
	{
		unsigned int currentPos = m_CaretPos + m_CaretPosOffset;

		IDWriteTextLayout* pLayout;
		TextRenderer::Get().CreateTextLayout(m_Parent->GetText(), &pLayout);

		if (currentPos > 0)
		{
			//Adopt trailing properties
			--currentPos;
		}

		// Get the family name
		m_CaretFormat.fontFamilyName[0] = '\0';
		pLayout->GetFontFamilyName(currentPos, &m_CaretFormat.fontFamilyName[0], ARRAYSIZE(m_CaretFormat.fontFamilyName));

		// Get the locale
		m_CaretFormat.localeName[0] = '\0';
		pLayout->GetLocaleName(currentPos, &m_CaretFormat.localeName[0], ARRAYSIZE(m_CaretFormat.localeName));

		// Get the remaining attributes...
		pLayout->GetFontWeight(currentPos, &m_CaretFormat.fontWeight);
		pLayout->GetFontStyle(currentPos, &m_CaretFormat.fontStyle);
		pLayout->GetFontStretch(currentPos, &m_CaretFormat.fontStretch);
		pLayout->GetFontSize(currentPos, &m_CaretFormat.fontSize);
		pLayout->GetUnderline(currentPos, &m_CaretFormat.hasUnderline);
		pLayout->GetStrikethrough(currentPos, &m_CaretFormat.hasStrikethrough);
	}

	D2D1_RECT_F TextBox::Caret::GetCaretRect()
	{
		D2D1_RECT_F rc{};

		float caretX, caretY;
		auto caretMetrics = TextRenderer::Get().HitTestTextPosition(m_Parent->GetText(), m_CaretPos, m_CaretPosOffset > 0, &caretX, &caretY);

		const float caretThickness = GetCaretThickness();

		rc.left = caretX - caretThickness / 2.0f;
		rc.right = rc.left + caretThickness;
		rc.top = caretY;
		rc.bottom = caretY + caretMetrics.height;

		return rc;
	}

	float TextBox::Caret::GetCaretThickness()
	{
		unsigned int caretIntThickness = 2;
		SystemParametersInfo(SPI_GETCARETWIDTH, 0, &caretIntThickness, FALSE);
		return (float)caretIntThickness;
	}
}


