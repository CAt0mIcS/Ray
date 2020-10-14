#include "pch.h"
#include "Caret.h"

#include "GUI/Controls/TextBox.h"

#include "Util/Direct2D.h"
#include "Util/Exceptions.h"

#include "Util/Util.h"

#include "GUI/Events/KeyboardEvent.h"
#include "GUI/Events/MouseEvent.h"


namespace GUI
{
	Caret::Caret(_In_ TextBox* parent)
		: m_Parent(parent), m_CaretPos(0), m_CaretPosOffset(0), m_CaretAnchor(0), m_CaretFormat{}, m_CurrentlySelecting(false)
	{

	}

	void Caret::Render()
	{
		RenderSelection();
		if (m_Parent->HasFocus())
		{
			RenderCaret();
		}
	}

	void Caret::SetSelection(_In_ MoveMode moveMode, _In_ unsigned int advance, _In_ bool extendSelection, _In_opt_ bool updateCaretFormat)
	{
		unsigned int line = UINT32_MAX;
		unsigned int absolutePosition = m_CaretPos + m_CaretPosOffset;
		unsigned int oldAbsolutePosition = absolutePosition;
		unsigned int oldCaretAnchor = m_CaretAnchor;

		auto& text = m_Parent->m_Text.text;

		switch (moveMode)
		{
		case MoveMode::Left:
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
		case MoveMode::Right:
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
		case MoveMode::LeftChar:
		{
			m_CaretPos = absolutePosition;
			m_CaretPos -= std::min(advance, absolutePosition);
			m_CaretPosOffset = 0;
			break;
		}
		case MoveMode::RightChar:
		{
			m_CaretPos = absolutePosition + advance;
			m_CaretPosOffset = 0;

			float caretX, caretY;
			DWrite::HitTestMetrics hitTestMetrics = TextRenderer::Get().HitTestTextPosition(
				m_Parent->GetText(),
				m_CaretPos,
				false,
				&caretX,
				&caretY
			);

			m_CaretPos = std::min(m_CaretPos, hitTestMetrics.textPosition + hitTestMetrics.length);

			break;
		}
		case MoveMode::Up:
		case MoveMode::Down:
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
			if (moveMode == MoveMode::Up)
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
			DWrite::HitTestMetrics hitTestMetrics = TextRenderer::Get().HitTestTextPosition(
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
		case MoveMode::LeftWord:
		case MoveMode::RightWord:
		{
			unsigned int clusterCount = 0;
			std::vector<DWrite::ClusterMetrics> clusterMetrics = TextRenderer::Get().GetClusterMetrics(m_Parent->GetText(), &clusterCount);

			m_CaretPos = absolutePosition;
			unsigned int clusterPosition = 0;
			unsigned int oldCaretPosition = m_CaretPos;

			if (moveMode == MoveMode::LeftWord)
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
			else // MoveMode::RightWord
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
		case MoveMode::AbsoluteLeading:
		{
			m_CaretPos = advance;
			m_CaretPosOffset = 0;
			break;
		}
		case MoveMode::AbsoluteTrailing:
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
		}
	}

	void Caret::AlignCaretToNearestCluster(_In_opt_ bool isTrailingHit, _In_opt_ bool skipZeroWidth)
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

	void Caret::UpdateCaretFormatting()
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

	Direct2D::RectF Caret::GetCaretRect()
	{
		Direct2D::RectF rc{};

		float caretX, caretY;
		auto caretMetrics = TextRenderer::Get().HitTestTextPosition(m_Parent->GetText(), m_CaretPos, m_CaretPosOffset > 0, &caretX, &caretY);

		const float caretThickness = GetCaretThickness();

		rc.left = caretX - caretThickness / 2.0f;
		rc.right = rc.left + caretThickness;
		rc.top = caretY;
		rc.bottom = caretY + caretMetrics.height;

		return rc;
	}

	float Caret::GetCaretThickness()
	{
		unsigned int caretIntThickness = 2;
		SystemParametersInfo(SPI_GETCARETWIDTH, 0, &caretIntThickness, FALSE);
		return (float)caretIntThickness;
	}

	void Caret::SetSelectionFromPoint(_In_ const Util::NPoint& pos, _In_ bool extendSelection)
	{
		BOOL isTrailingHit;
		BOOL isInside;

		auto caretMetrics = TextRenderer::Get().HitTestPoint(m_Parent->GetText(), &isTrailingHit, &isInside);

		SetSelection(
			isTrailingHit ? MoveMode::AbsoluteTrailing : MoveMode::AbsoluteLeading,
			caretMetrics.textPosition,
			extendSelection
		);
	}

	void Caret::OnMouseButtonPressed(_In_ MouseButtonPressedEvent& e)
	{
		if (e.GetButton() == MouseButton::Left)
		{
			m_CurrentlySelecting = true;
			SetSelectionFromPoint(Mouse::GetTransformedPos(), false);
		}
	}

	void Caret::OnMouseButtonReleased(_In_ MouseButtonReleasedEvent& e)
	{
		m_CurrentlySelecting = false;
	}

	void Caret::OnMouseMove(_In_ MouseMoveEvent& e)
	{
		if (m_CurrentlySelecting)
		{
			SetSelectionFromPoint(e.GetPos(), true);
		}
	}

	void Caret::OnCharEvent(_In_ CharEvent& e)
	{
		//Allow normal characters and tab
		if (e.GetKeyCode() >= 0x20 || e.GetKeyCode() == 9)
		{
			//Replace existing selection
			DeleteSelection();

			UINT32 charsLength = 1;
			wchar_t chars[2] = { (wchar_t)e.GetKeyCode(), 0 };

			m_Parent->m_Text.text.insert(m_CaretPos + m_CaretPosOffset, chars);
			SetSelection(MoveMode::Right, charsLength, false, false);
		}
	}
	
	void Caret::OnKeyPressed(_In_ KeyPressedEvent& e)
	{
		switch (e.GetKeyCode())
		{
		case VK_RETURN:
		{
			unsigned int absolutePosition = m_CaretPos + m_CaretPosOffset;

			DeleteSelection();
			m_Parent->m_Text.text.insert(absolutePosition, L"\r\n");
			SetSelection(MoveMode::AbsoluteLeading, absolutePosition + 2, false, false);
			break;
		}
		case VK_BACK:
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
				SetSelection(MoveMode::LeftChar, count, false);
				m_Parent->m_Text.text.erase(m_CaretPos, count);
			}
			break;
		}
		case VK_LEFT:
		{
			SetSelection(Keyboard::IsKeyPressed(VK_CONTROL) ? MoveMode::LeftWord : MoveMode::Left, 1, Keyboard::IsKeyPressed(VK_SHIFT));
			break;
		}
		case VK_RIGHT:
		{
			SetSelection(Keyboard::IsKeyPressed(VK_CONTROL) ? MoveMode::RightWord : MoveMode::Right, 1, Keyboard::IsKeyPressed(VK_SHIFT));
			unsigned int absolutePosition = m_CaretPos + m_CaretPosOffset;
			break;
		}
		case VK_UP:
		{
			SetSelection(MoveMode::Up, 1, Keyboard::IsKeyPressed(VK_SHIFT));
			unsigned int absolutePosition = m_CaretPos + m_CaretPosOffset;
			break;
		}
		case VK_DOWN:
		{
			SetSelection(MoveMode::Down, 1, Keyboard::IsKeyPressed(VK_SHIFT));
			unsigned int absolutePosition = m_CaretPos + m_CaretPosOffset;
			break;
		}
		}
	}

	void Caret::GetLineFromPosition(
		_In_ const DWrite::LineMetrics* lineMetrics, 
		_In_ unsigned int lineCount, 
		_In_ unsigned int textPosition, 
		_Out_ unsigned int* lineOut, 
		_Out_ unsigned int* linePositionOut
		)
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
	
	void Caret::RenderCaret()
	{
		auto caretRect = GetCaretRect();
		float caretHeight = caretRect.bottom - caretRect.top;

		auto lineMetrics = TextRenderer::Get().GetLineMetrics(m_Parent->GetText());
		
		unsigned int line;
		unsigned int linePosition;
		GetLineFromPosition(
			&lineMetrics.front(),
			(unsigned int)lineMetrics.size(),
			m_CaretPos,
			&line,
			&linePosition
		);

		float xOffset = m_Parent->GetSize().width / 30.0f;
		caretRect.left += (m_Parent->GetPos().x + xOffset);

		float yOffset;
		if (m_Parent->m_IsMultiline)
		{
			yOffset = m_Parent->GetSize().height / 10.0f;
		}
		else
		{
			yOffset = m_Parent->GetSize().height / 2.0f - m_Parent->GetText().fontSize / 2.0f;
		}

		caretRect.top += m_Parent->GetPos().y + yOffset;
		caretRect.right = caretRect.left + GetCaretThickness();
		caretRect.bottom += caretRect.top - (line * caretHeight);

		Renderer::Get().RenderRect(caretRect, Constants::g_DefaultCaretColor);
	}

	void Caret::RenderSelection()
	{
		IDWriteTextLayout* pLayout;
		TextRenderer::Get().CreateTextLayout(m_Parent->GetText(), &pLayout);

		DWRITE_TEXT_RANGE caretRange = GetSelectionRange();
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
		std::vector<DWrite::HitTestMetrics> hitTestMetrics(actualHitTestCount);

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
			float xOffset = m_Parent->GetSize().width / 30.0f;
			float yOffset;
			if (m_Parent->IsMultiline())
			{
				yOffset = m_Parent->GetSize().height / 10.0f;
			}
			else
			{
				yOffset = m_Parent->GetSize().height / 2.0f - m_Parent->GetText().fontSize / 2.0f;
			}

			for (size_t i = 0; i < actualHitTestCount; ++i)
			{
				const DWrite::HitTestMetrics& htm = hitTestMetrics[i];
				Direct2D::RectF highlightRect{};
				highlightRect.left = htm.left + m_Parent->GetPos().x + xOffset;
				highlightRect.top = htm.top + m_Parent->GetPos().y + yOffset;
				highlightRect.right = highlightRect.left + htm.width;
				highlightRect.bottom = highlightRect.top + htm.height;

				Renderer::Get().RenderRect(highlightRect, Constants::g_DefaultSelectionRectColor);
			}
		}

		Util::Release(&pLayout);
	}

	void Caret::DeleteSelection()
	{
		DWRITE_TEXT_RANGE selectionRange = GetSelectionRange();
		if (selectionRange.length <= 0)
			return;

		m_Parent->m_Text.text.erase(selectionRange.startPosition, selectionRange.length);
		SetSelection(MoveMode::AbsoluteLeading, selectionRange.startPosition, false);

	}

	DWRITE_TEXT_RANGE Caret::GetSelectionRange()
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
	
	void Caret::ClearSelection()
	{
		m_CaretAnchor = m_CaretPos;
		m_CaretPosOffset = 0;
	}
}
