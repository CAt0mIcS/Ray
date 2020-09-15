#include "pch.h"
#include "EditableDWriteLayout.h"

#include "Util/Direct2D.h"
#include "Util/Exceptions.h"


namespace GUI
{
	//TODO: Check if m_pFactory will be added as ref correctly
	EditableDWriteLayout::EditableDWriteLayout(IDWriteFactory* factory)
		: m_pFactory(Util::Aquire(factory))
	{
	}

	EditableDWriteLayout::~EditableDWriteLayout()
	{
		Util::Release(&m_pFactory);
	}
	
	void EditableDWriteLayout::InsertTextAt(
		_Inout_ IDWriteTextLayout*& currentLayout,
		_Inout_ std::wstring& text,
		_In_ unsigned int position,
		_In_ const std::wstring& textToInsert,
		_In_ unsigned int textToInsertLength,
		_In_opt_ CaretFormat* pCaretFormat
	)
	{
		IDWriteTextLayout* oldLayout = Util::Aquire(currentLayout);
		UINT32 oldTextLength = (UINT32)text.length();
		position = std::min(position, oldTextLength);

		text.insert(position, textToInsert, textToInsertLength);
		RecreateLayout(currentLayout, text);

		IDWriteTextLayout* newLayout = currentLayout;

		CopyGlobalProperties(oldLayout, newLayout);

		// For each property, get the position range and apply it to the old text.
		if (position == 0)
		{
			// Inserted text
			CopySinglePropertyRange(oldLayout, 0, newLayout, 0, textToInsertLength);

			// The rest of the text
			CopyRangedProperties(oldLayout, 0, oldTextLength, textToInsertLength, newLayout);
		}
		else
		{
			// 1st block
			CopyRangedProperties(oldLayout, 0, position, 0, newLayout);

			// Inserted text
			CopySinglePropertyRange(oldLayout, position - 1, newLayout, position, textToInsertLength, pCaretFormat);

			// Last block (if it exists)
			CopyRangedProperties(oldLayout, position, oldTextLength, textToInsertLength, newLayout);
		}

		// Copy trailing end.
		CopySinglePropertyRange(oldLayout, oldTextLength, newLayout, static_cast<UINT32>(text.length()), UINT32_MAX);

		Util::Release(&oldLayout);
	}

	void EditableDWriteLayout::RemoveTextAt(
		_Inout_ IDWriteTextLayout*& currentLayout,
		_Inout_ std::wstring& text,
		_In_ UINT32 position,
		_In_ UINT32 lengthToRemove
	)
	{
		// Removes text and shifts all formatting.

		// copy all the old formatting.
		IDWriteTextLayout* oldLayout = Util::Aquire(currentLayout);
		UINT32 oldTextLength = static_cast<UINT32>(text.length());

		// Remove the old text and recreate the new text layout.
		text.erase(position, lengthToRemove);
		RecreateLayout(currentLayout, text);

		IDWriteTextLayout* newLayout = currentLayout;

		CopyGlobalProperties(oldLayout, newLayout);

		if (position == 0)
		{
			// The rest of the text
			CopyRangedProperties(oldLayout, lengthToRemove, oldTextLength, lengthToRemove, newLayout, true);
		}
		else
		{
			// 1st block
			CopyRangedProperties(oldLayout, 0, position, 0, newLayout, true);

			// Last block (if it exists, we increment past the deleted text)
			CopyRangedProperties(oldLayout, position + lengthToRemove, oldTextLength, lengthToRemove, newLayout, true);
		}
		CopySinglePropertyRange(oldLayout, oldTextLength, newLayout, static_cast<UINT32>(text.length()), UINT32_MAX);
		Util::Release(&oldLayout);
	}

	void EditableDWriteLayout::RecreateLayout(_Inout_ IDWriteTextLayout*& currentLayout, _In_ const std::wstring& text)
	{
		IDWriteTextLayout* newLayout = nullptr;

		NPE_THROW_GFX_EXCEPT(m_pFactory->CreateTextLayout(
			text.c_str(),
			static_cast<UINT32>(text.length()),
			currentLayout,
			currentLayout->GetMaxWidth(),
			currentLayout->GetMaxHeight(),
			&newLayout
		), "Failed to recreate DWriteTextLayout");

		Util::Attach(&currentLayout, Util::Detach(&newLayout));

		Util::Release(&newLayout);
	}

	//from https://github.com/microsoft/Windows-classic-samples/tree/master/Samples/Win7Samples/multimedia/DirectWrite/PadWrite (modified)
	void EditableDWriteLayout::CopyGlobalProperties(_In_ IDWriteTextLayout* oldLayout, _In_ IDWriteTextLayout* newLayout)
	{
		// Copies global properties that are not range based.

		newLayout->SetTextAlignment(oldLayout->GetTextAlignment());
		newLayout->SetParagraphAlignment(oldLayout->GetParagraphAlignment());
		newLayout->SetWordWrapping(oldLayout->GetWordWrapping());
		newLayout->SetReadingDirection(oldLayout->GetReadingDirection());
		newLayout->SetFlowDirection(oldLayout->GetFlowDirection());
		newLayout->SetIncrementalTabStop(oldLayout->GetIncrementalTabStop());

		DWRITE_TRIMMING trimmingOptions = {};
		IDWriteInlineObject* inlineObject = nullptr;
		oldLayout->GetTrimming(&trimmingOptions, &inlineObject);
		newLayout->SetTrimming(&trimmingOptions, inlineObject);
		Util::Release(&inlineObject);

		DWRITE_LINE_SPACING_METHOD lineSpacingMethod = DWRITE_LINE_SPACING_METHOD_DEFAULT;
		FLOAT lineSpacing = 0;
		FLOAT baseline = 0;
		oldLayout->GetLineSpacing(&lineSpacingMethod, &lineSpacing, &baseline);
		newLayout->SetLineSpacing(lineSpacingMethod, lineSpacing, baseline);
	}

	//from https://github.com/microsoft/Windows-classic-samples/tree/master/Samples/Win7Samples/multimedia/DirectWrite/PadWrite (modified)
	void EditableDWriteLayout::CopyRangedProperties(_In_ IDWriteTextLayout* oldLayout, _In_ unsigned int startPos, _In_ unsigned int endPos, _In_ unsigned int newLayoutTextOffset, _In_ IDWriteTextLayout* newLayout, _In_opt_ bool isOffsetNegative)
	{
		// Copies properties that set on ranges.

		auto CalculateRangeLengthAt = [](IDWriteTextLayout* layout, UINT32 pos)
		{
			// Determines the length of a block of similarly formatted properties.

			// Use the first getter to get the range to increment the current position.
			DWRITE_TEXT_RANGE incrementAmount = { pos, 1 };
			DWRITE_FONT_WEIGHT weight = DWRITE_FONT_WEIGHT_NORMAL;

			layout->GetFontWeight(
				pos,
				&weight,
				&incrementAmount
			);

			UINT32 rangeLength = incrementAmount.length - (pos - incrementAmount.startPosition);
			return rangeLength;
		};


		UINT32 currentPos = startPos;
		while (currentPos < endPos)
		{
			UINT32 rangeLength = CalculateRangeLengthAt(oldLayout, currentPos);
			rangeLength = std::min(rangeLength, endPos - currentPos);
			if (isOffsetNegative)
			{
				CopySinglePropertyRange(
					oldLayout,
					currentPos,
					newLayout,
					currentPos - newLayoutTextOffset,
					rangeLength
				);
			}
			else
			{
				CopySinglePropertyRange(
					oldLayout,
					currentPos,
					newLayout,
					currentPos + newLayoutTextOffset,
					rangeLength
				);
			}
			currentPos += rangeLength;
		}
	}
	
	//from https://github.com/microsoft/Windows-classic-samples/tree/master/Samples/Win7Samples/multimedia/DirectWrite/PadWrite (modified)
	void EditableDWriteLayout::CopySinglePropertyRange(_In_ IDWriteTextLayout* oldLayout, _In_ unsigned int startPosForOld, _In_ IDWriteTextLayout* newLayout, _In_ unsigned int startPosForNew, _In_ unsigned int length, _In_opt_ CaretFormat* caretFormat)
	{
		// Copies a single range of similar properties, from one old layout
		// to a new one.

		DWRITE_TEXT_RANGE range = { startPosForNew,  std::min(length, UINT32_MAX - startPosForNew) };

		// font collection
		IDWriteFontCollection* fontCollection = nullptr;
		oldLayout->GetFontCollection(startPosForOld, &fontCollection);
		newLayout->SetFontCollection(fontCollection, range);
		Util::Release(&fontCollection);

		if (caretFormat)
		{
			newLayout->SetFontFamilyName(caretFormat->fontFamilyName.c_str(), range);
			newLayout->SetLocaleName(caretFormat->localeName.c_str(), range);
			newLayout->SetFontWeight(caretFormat->fontWeight, range);
			newLayout->SetFontStyle(caretFormat->fontStyle, range);
			newLayout->SetFontStretch(caretFormat->fontStretch, range);
			newLayout->SetFontSize(caretFormat->fontSize, range);
			newLayout->SetUnderline(caretFormat->hasUnderline, range);
			newLayout->SetStrikethrough(caretFormat->hasStrikethrough, range);
		}
		else
		{
			// font family
			wchar_t fontFamilyName[100];
			fontFamilyName[0] = '\0';
			oldLayout->GetFontFamilyName(startPosForOld, &fontFamilyName[0], ARRAYSIZE(fontFamilyName));
			newLayout->SetFontFamilyName(fontFamilyName, range);

			// weight/width/slope
			DWRITE_FONT_WEIGHT weight = DWRITE_FONT_WEIGHT_NORMAL;
			DWRITE_FONT_STYLE style = DWRITE_FONT_STYLE_NORMAL;
			DWRITE_FONT_STRETCH stretch = DWRITE_FONT_STRETCH_NORMAL;
			oldLayout->GetFontWeight(startPosForOld, &weight);
			oldLayout->GetFontStyle(startPosForOld, &style);
			oldLayout->GetFontStretch(startPosForOld, &stretch);

			newLayout->SetFontWeight(weight, range);
			newLayout->SetFontStyle(style, range);
			newLayout->SetFontStretch(stretch, range);

			// font size
			FLOAT fontSize = 12.0;
			oldLayout->GetFontSize(startPosForOld, &fontSize);
			newLayout->SetFontSize(fontSize, range);

			// underline and strikethrough
			BOOL value = FALSE;
			oldLayout->GetUnderline(startPosForOld, &value);
			newLayout->SetUnderline(value, range);
			oldLayout->GetStrikethrough(startPosForOld, &value);
			newLayout->SetStrikethrough(value, range);

			// locale
			wchar_t localeName[LOCALE_NAME_MAX_LENGTH];
			localeName[0] = '\0';
			oldLayout->GetLocaleName(startPosForOld, &localeName[0], ARRAYSIZE(localeName));
			newLayout->SetLocaleName(localeName, range);
		}

		// drawing effect
		IUnknown* drawingEffect = nullptr;
		oldLayout->GetDrawingEffect(startPosForOld, &drawingEffect);
		newLayout->SetDrawingEffect(drawingEffect, range);
		Util::Release(&drawingEffect);

		// inline object
		IDWriteInlineObject* inlineObject = nullptr;
		oldLayout->GetInlineObject(startPosForOld, &inlineObject);
		newLayout->SetInlineObject(inlineObject, range);
		Util::Release(&inlineObject);

		// typography
		IDWriteTypography* typography = nullptr;
		oldLayout->GetTypography(startPosForOld, &typography);
		newLayout->SetTypography(typography, range);
		Util::Release(&typography);
	}
}


