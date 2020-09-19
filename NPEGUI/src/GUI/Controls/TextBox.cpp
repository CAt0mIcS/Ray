#include "pch.h"
#include "TextBox.h"

#include "GUI/Graphics/Renderer.h"
#include "GUI/Graphics/TextRenderer.h"

#include "GUI/Events/MouseEvent.h"
#include "GUI/Events/KeyboardEvent.h"
#include "GUI/Events/ApplicationEvent.h"

#include "Util/Exceptions.h"
#include "Util/Direct2D.h"

/**
* QUESTION:
*	Should I be designing something like this GUI library only for this project or should I make it usable in another one?
*	Should I write class declarations (class Node;) in header files instead of including them
*/



namespace GUI
{
	TextBox::TextBox(Control* parent)
		: Control(parent), m_Text{}, m_IsMultiline(false), m_Caret(this)
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
			RenderSelection();
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
			OnMouseButtonPressed((MouseButtonPressedEvent&)e);
		}
		else if (e.GetType() == EventType::MouseButtonReleasedEvent)
		{
			OnMouseButtonReleased((MouseButtonReleasedEvent&)e);
		}
		else if (e.GetType() == EventType::KeyPressedEvent)
		{
			OnKeyPressed((KeyPressedEvent&)e);
		}
		else if (e.GetType() == EventType::CharEvent)
		{
			m_Caret.OnCharEvent((CharEvent&)e);
		}
		else if (e.GetType() == EventType::SetCursorEvent)
		{
			/**
			* Mouse is already guaranteed to be on the control, we don't need to check here
			* Default cursor is automatically restored when exiting the control
			*/
			SetCursor(LoadCursor(NULL, IDC_IBEAM));
			return true;
		}
		
		return false;
	}

	//TODO: Fix multiline text causing caret to be too long (Position is translated using the top left corner of the first line of text!)ite.
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

		Renderer::Get().RenderRect(caretRect, g_DefaultCaretColor);
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

	void TextBox::RenderSelection()
	{
		IDWriteTextLayout* pLayout;
		TextRenderer::Get().CreateTextLayout(m_Text, &pLayout);

		DWRITE_TEXT_RANGE caretRange = m_Caret.GetSelectionRange();
		UINT32 actualHitTestCount = 0;

		if (caretRange.length > 0)
		{
			pLayout->HitTestTextRange(
				caretRange.startPosition,
				caretRange.length,
				0, // x
				0, // y
				nullptr,
				0, // metrics count
				&actualHitTestCount
			);
		}

		// Allocate enough room to return all hit-test metrics.
		std::vector<DWRITE_HIT_TEST_METRICS> hitTestMetrics(actualHitTestCount);

		if (caretRange.length > 0)
		{
			pLayout->HitTestTextRange(
				caretRange.startPosition,
				caretRange.length,
				0, // x
				0, // y
				&hitTestMetrics[0],
				(UINT32)hitTestMetrics.size(),
				&actualHitTestCount
			);
		}

		// Draw the selection ranges behind the text.
		if (actualHitTestCount > 0)
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

			for (size_t i = 0; i < actualHitTestCount; ++i)
			{
				const DWRITE_HIT_TEST_METRICS& htm = hitTestMetrics[i];
				D2D1_RECT_F highlightRect{};
				highlightRect.left = htm.left + m_Pos.x + xOffset;
				highlightRect.top = htm.top + m_Pos.y + yOffset;
				highlightRect.right = highlightRect.left + htm.width;
				highlightRect.bottom = highlightRect.top + htm.height;

				Renderer::Get().RenderRect(highlightRect, g_DefaultSelectionRectColor);
			}
		}

		Util::Release(&pLayout);
	}

	std::optional<std::pair<Util::NPoint, Util::NSize>> TextBox::CalculateLayout(const Util::NPoint& parentPos, const Util::NSize& parentSize)
	{
		return { {{}, {}} };
	}

	void TextBox::OnMouseButtonPressed(MouseButtonPressedEvent& e)
	{
		m_Caret.OnMouseButtonPressed(e);
	}

	void TextBox::OnMouseButtonReleased(MouseButtonReleasedEvent& e)
	{
		if (e.GetButton() == MouseButton::Left)
		{
			m_Caret.OnMouseButtonReleased(e);
		}
	}

	void TextBox::OnKeyPressed(KeyPressedEvent& e)
	{
		switch (e.GetKeyCode())
		{
		case VK_RETURN:
		{
			m_Caret.OnReturnPressed(e);
			break;
		}
		case VK_BACK:
		{
			m_Caret.OnBackPressed(e);
			break;
		}
		case VK_LEFT:
		{
			m_Caret.OnLeftPressed(e);
			break;
		}
		case VK_RIGHT:
		{
			m_Caret.OnRightPressed(e);
			break;
		}
		case VK_UP:
		{
			m_Caret.OnUpPressed(e);
			break;
		}
		case VK_DOWN:
		{
			m_Caret.OnDownPressed(e);
			break;
		}
		}
	}


	/******************
	*****  Caret  *****
	*******************/
	TextBox::Caret::Caret(TextBox* parent)
		: m_Parent(parent), m_CaretPos(0), m_CaretPosOffset(0), m_CaretAnchor(0), m_CurrentlySelecting(false), m_CaretFormat{}
	{

	}

	void TextBox::Caret::SetSelection(SetSelectionMode moveMode, unsigned int advance, bool extendSelection, bool updateCaretFormat)
	{
		unsigned int line = UINT32_MAX;
		unsigned int absolutePosition = m_CaretPos + m_CaretPosOffset;
		unsigned int oldAbsolutePosition = absolutePosition;
		unsigned int oldCaretAnchor = m_CaretAnchor;

		auto& text = m_Parent->m_Text.text;

		switch (moveMode)
		{
		case SetSelectionMode::Left:
		{
			m_CaretPos += m_CaretPosOffset;
			if (m_CaretPos > 0)
			{
				--m_CaretPos;
				AlignCaretToNearestCluster(false, true);
				absolutePosition = m_CaretPos + m_CaretPosOffset;
				if (absolutePosition >= 1
					&& absolutePosition < text.size()
					&& text[absolutePosition - 1] == '\r'
					&& text[absolutePosition] == '\n')
				{
					m_CaretPos = absolutePosition - 1;
					AlignCaretToNearestCluster(false, true);
				}
			}
			break;
		}
		case SetSelectionMode::Right:
		{
			m_CaretPos = absolutePosition;
			AlignCaretToNearestCluster(true, true);

			absolutePosition = m_CaretPos + m_CaretPosOffset;
			if (absolutePosition >= 1
				&& absolutePosition < text.size()
				&& text[absolutePosition - 1] == '\r'
				&& text[absolutePosition] == '\n')
			{
				m_CaretPos = absolutePosition + 1;
				AlignCaretToNearestCluster(false, true);
			}

			break;
		}
		case SetSelectionMode::LeftChar:
		{
			m_CaretPos = absolutePosition;
			m_CaretPos -= std::min(advance, absolutePosition);
			m_CaretPosOffset = 0;
			break;
		}
		case SetSelectionMode::RightChar:
		{
			m_CaretPos = absolutePosition + advance;
			m_CaretPosOffset = 0;

			float caretX, caretY;
			DWRITE_HIT_TEST_METRICS hitTestMetrics = TextRenderer::Get().HitTestTextPosition(
				m_Parent->GetText(),
				m_CaretPos,
				false,
				&caretX,
				&caretY
			);

			m_CaretPos = std::min(m_CaretPos, hitTestMetrics.textPosition + hitTestMetrics.length);

			break;
		}
		case SetSelectionMode::Up:
		case SetSelectionMode::Down:
		{
			// Retrieve the line metrics to figure out what line we are on.
			auto lineMetrics = TextRenderer::Get().GetLineMetrics(m_Parent->GetText());

			unsigned int linePosition;
			GetLineFromPosition(
				&lineMetrics.front(),
				(unsigned int)lineMetrics.size(),
				m_CaretPos,
				&line,
				&linePosition
			);

			// Move up a line or down
			if (moveMode == SetSelectionMode::Up)
			{
				if (line <= 0)
					break; // already top line
				line--;
				linePosition -= lineMetrics[line].length;
			}
			else
			{
				linePosition += lineMetrics[line].length;
				line++;
				if (line >= lineMetrics.size())
					break; // already bottom line
			}

			// To move up or down, we need three hit-testing calls to determine:
			// 1. The x of where we currently are.
			// 2. The y of the new line.
			// 3. New text position from the determined x and y.
			// This is because the characters are variable size.

			float caretX, caretY, dummyX;

			// Get x of current text position
			DWRITE_HIT_TEST_METRICS hitTestMetrics = TextRenderer::Get().HitTestTextPosition(
				m_Parent->GetText(),
				m_CaretPos,
				m_CaretPosOffset > 0,
				&caretX,
				&caretY
			);

			// Get y of new position
			hitTestMetrics = TextRenderer::Get().HitTestTextPosition(
				m_Parent->GetText(),
				linePosition,
				false,
				&dummyX,
				&caretY
			);

			// Now get text position of new x,y.
			IDWriteTextLayout* pLayout;
			TextRenderer::Get().CreateTextLayout(m_Parent->GetText(), &pLayout);

			BOOL isInside, isTrailingHit;
			pLayout->HitTestPoint(
				caretX,
				caretY,
				&isTrailingHit,
				&isInside,
				&hitTestMetrics
			);
			Util::Release(&pLayout);

			m_CaretPos = hitTestMetrics.textPosition;
			m_CaretPosOffset = isTrailingHit ? (hitTestMetrics.length > 0) : 0;

			break;
		}
		case SetSelectionMode::LeftWord:
		case SetSelectionMode::RightWord:
		{
			unsigned int clusterCount = 0;
			std::vector<DWRITE_CLUSTER_METRICS> clusterMetrics = TextRenderer::Get().GetClusterMetrics(m_Parent->GetText(), &clusterCount);

			m_CaretPos = absolutePosition;
			unsigned int clusterPosition = 0;
			unsigned int oldCaretPosition = m_CaretPos;

			if (moveMode == SetSelectionMode::LeftWord)
			{
				m_CaretPos = 0;
				m_CaretPosOffset = 0;
				for (unsigned int cluster = 0; cluster < clusterCount; ++cluster)
				{
					clusterPosition += clusterMetrics[cluster].length;
					if (clusterMetrics[cluster].canWrapLineAfter)
					{
						if (clusterPosition >= oldCaretPosition)
							break;

						m_CaretPos = clusterPosition;
					}
				}
			}
			else // SetSelectionMode::RightWord
			{
				for (unsigned int cluster = 0; cluster < clusterCount; ++cluster)
				{
					unsigned int clusterLength = clusterMetrics[cluster].length;
					m_CaretPos = clusterPosition;
					m_CaretPosOffset = clusterLength;
					if (clusterPosition >= oldCaretPosition && clusterMetrics[cluster].canWrapLineAfter)
						break;

					clusterPosition += clusterLength;
				}
			}

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
			InvalidateRect(Renderer::Get().GetNativeWindow(), nullptr, TRUE);
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
		NPE_THROW_GFX_EXCEPT(pLayout->GetFontFamilyName(currentPos, &m_CaretFormat.fontFamilyName[0], ARRAYSIZE(m_CaretFormat.fontFamilyName)), "Failed to get font family name");

		// Get the locale
		m_CaretFormat.localeName[0] = '\0';
		NPE_THROW_GFX_EXCEPT(pLayout->GetLocaleName(currentPos, &m_CaretFormat.localeName[0], ARRAYSIZE(m_CaretFormat.localeName)), "failed to get locale name");

		// Get the remaining attributes...
		NPE_THROW_GFX_EXCEPT(pLayout->GetFontWeight(currentPos, &m_CaretFormat.fontWeight), "failed to get font weight");
		NPE_THROW_GFX_EXCEPT(pLayout->GetFontStyle(currentPos, &m_CaretFormat.fontStyle), "failed to get font style");
		NPE_THROW_GFX_EXCEPT(pLayout->GetFontStretch(currentPos, &m_CaretFormat.fontStretch), "failed to get font stretch");
		NPE_THROW_GFX_EXCEPT(pLayout->GetFontSize(currentPos, &m_CaretFormat.fontSize), "failed to get font size");
		NPE_THROW_GFX_EXCEPT(pLayout->GetUnderline(currentPos, &m_CaretFormat.hasUnderline), "failed to get underline");
		NPE_THROW_GFX_EXCEPT(pLayout->GetStrikethrough(currentPos, &m_CaretFormat.hasStrikethrough), "failed to get strikethrough");
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

	void TextBox::Caret::OnMouseButtonPressed(MouseButtonPressedEvent& e)
	{
		if (e.GetButton() == MouseButton::Left)
		{
			m_CurrentlySelecting = true;

			bool extendSelection = false;
			BOOL isTrailingHit;
			BOOL isInside;

			auto caretMetrics = TextRenderer::Get().HitTestPoint(m_Parent->GetText(), &isTrailingHit, &isInside);

			SetSelection(
				isTrailingHit ? SetSelectionMode::AbsoluteTrailing : SetSelectionMode::AbsoluteLeading,
				caretMetrics.textPosition,
				extendSelection
			);
		}
	}

	void TextBox::Caret::OnMouseButtonReleased(MouseButtonReleasedEvent& e)
	{
		m_CurrentlySelecting = false;
	}

	void TextBox::Caret::OnCharEvent(CharEvent& e)
	{
		//Allow normal characters and tab
		if (e.GetKeyCode() >= 0x20 || e.GetKeyCode() == 9)
		{
			//Replace existing selection
			DeleteSelection();

			UINT32 charsLength = 1;
			wchar_t chars[2] = { (wchar_t)e.GetKeyCode(), 0 };

			m_Parent->m_Text.text.insert(m_CaretPos + m_CaretPosOffset, chars);
			SetSelection(SetSelectionMode::Right, charsLength, false, false);
		}
	}

	void TextBox::Caret::OnReturnPressed(KeyPressedEvent& e)
	{
		unsigned int absolutePosition = m_CaretPos + m_CaretPosOffset;

		DeleteSelection();
		//TODO: Copy properties from next text
		m_Parent->m_Text.text.insert(absolutePosition, L"\r\n");
		SetSelection(SetSelectionMode::AbsoluteLeading, absolutePosition + 2, false, false);
	}

	void TextBox::Caret::OnBackPressed(KeyPressedEvent& e)
	{
		unsigned int absolutePosition = m_CaretPos + m_CaretPosOffset;
		auto& text = m_Parent->GetText().text;

		auto IsLowSurrogate = [](wchar_t ch) { return (ch & 0xFC00) == 0xDC00; };
		auto IsHighSurrogate = [](wchar_t ch) { return (ch & 0xFC00) == 0xD800; };

		if (absolutePosition != m_CaretAnchor)
		{
			DeleteSelection();
		}
		else if (absolutePosition > 0)
		{
			unsigned int count = 1;
			if (absolutePosition >= 2 && absolutePosition <= text.size())
			{
				wchar_t charBackOne = text[absolutePosition - 1];
				wchar_t charBackTwo = text[absolutePosition - 2];
				if ((IsLowSurrogate(charBackOne) && IsHighSurrogate(charBackTwo)) || (charBackOne == L'\n' && charBackTwo == L'\r'))
				{
					count = 2;
				}
			}
			SetSelection(SetSelectionMode::LeftChar, count, false);
			m_Parent->m_Text.text.erase(m_CaretPos, count);
		}

	}

	void TextBox::Caret::OnLeftPressed(KeyPressedEvent& e)
	{
		SetSelection(Keyboard::IsKeyPressed(VK_CONTROL) ? SetSelectionMode::LeftWord : SetSelectionMode::Left, 1, Keyboard::IsKeyPressed(VK_SHIFT));
	}

	void TextBox::Caret::OnRightPressed(KeyPressedEvent& e)
	{
		SetSelection(Keyboard::IsKeyPressed(VK_CONTROL) ? SetSelectionMode::RightWord : SetSelectionMode::Right, 1, Keyboard::IsKeyPressed(VK_SHIFT));
		unsigned int absolutePosition = m_CaretPos + m_CaretPosOffset;
	}

	void TextBox::Caret::OnUpPressed(KeyPressedEvent& e)
	{
		SetSelection(SetSelectionMode::Up, 1, Keyboard::IsKeyPressed(VK_SHIFT));
		unsigned int absolutePosition = m_CaretPos + m_CaretPosOffset;
	}

	void TextBox::Caret::OnDownPressed(KeyPressedEvent& e)
	{
		SetSelection(SetSelectionMode::Down, 1, Keyboard::IsKeyPressed(VK_SHIFT));
		unsigned int absolutePosition = m_CaretPos + m_CaretPosOffset;
	}

	void TextBox::Caret::DeleteSelection()
	{
		DWRITE_TEXT_RANGE selectionRange = GetSelectionRange();
		if (selectionRange.length <= 0)
			return;

		m_Parent->m_Text.text.erase(selectionRange.startPosition, selectionRange.length);
		SetSelection(SetSelectionMode::AbsoluteLeading, selectionRange.startPosition, false);

	}

	DWRITE_TEXT_RANGE TextBox::Caret::GetSelectionRange()
	{
		unsigned int caretBegin = m_CaretAnchor;
		unsigned int caretEnd = m_CaretPos + m_CaretPosOffset;
		if (caretBegin > caretEnd)
			std::swap(caretBegin, caretEnd);

		unsigned int textLength = (unsigned int)m_Parent->GetText().text.size();
		caretBegin = std::min(caretBegin, textLength);
		caretEnd = std::min(caretEnd, textLength);

		return { caretBegin, caretEnd - caretBegin };
	}

	void TextBox::Caret::GetLineFromPosition(_In_ const DWRITE_LINE_METRICS* lineMetrics, _In_ unsigned int lineCount, _In_ unsigned int textPosition, _Out_ unsigned int* lineOut, _Out_ unsigned int* linePositionOut)
	{
		unsigned int line = 0;
		unsigned int linePosition = 0;
		unsigned int nextLinePosition = 0;
		for (; line < lineCount; ++line)
		{
			linePosition = nextLinePosition;
			nextLinePosition = linePosition + lineMetrics[line].length;
			if (nextLinePosition > textPosition)
			{
				// The next line is beyond the desired text position,
				// so it must be in the current line.
				break;
			}
		}
		*linePositionOut = linePosition;
		*lineOut = std::min(line, lineCount - 1);
	}

}


