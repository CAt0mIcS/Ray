#pragma once

#include "GUIBase.h"
#include "Win.h"

#include "Util/Util.h"

#include <dwrite.h>



namespace GUI
{
	class GUI_API EditableDWriteLayout
	{
	public:
		struct CaretFormat
		{
			std::wstring fontFamilyName;
			std::wstring localeName;
			float fontSize;
			DWRITE_FONT_WEIGHT fontWeight;
			DWRITE_FONT_STRETCH fontStretch;
			DWRITE_FONT_STYLE fontStyle;
			Util::NColor color;
			bool hasUnderline;
			bool hasStrikethrough;
		};

		EditableDWriteLayout(IDWriteFactory* factory);

		~EditableDWriteLayout();

		void InsertTextAt(
			_Inout_ IDWriteTextLayout*& currentLayout,
			_Inout_ std::wstring& text,
			_In_ unsigned int position,
			_In_ const std::wstring& textToInsert,
			_In_ unsigned int textToInsertLength,
			_In_opt_ CaretFormat* pCaretFormat = nullptr
		);

		void RemoveTextAt(
			_Inout_ IDWriteTextLayout*& currentLayout,
			_Inout_ std::wstring& text,
			_In_ unsigned int position,
			_In_ unsigned int lengthToRemove
		);

	private:
		void RecreateLayout(
			_Inout_ IDWriteTextLayout*& currentLayout,
			_In_ const std::wstring& text
		);

		static void CopyGlobalProperties(
			_In_ IDWriteTextLayout* oldLayout,
			_In_ IDWriteTextLayout* newLayout
		);

		static void CopyRangedProperties(
			_In_ IDWriteTextLayout* oldLayout,
			_In_ unsigned int startPos,
			_In_ unsigned int endPos,
			_In_ unsigned int newLayoutTextOffset,
			_In_ IDWriteTextLayout* newLayout,
			_In_opt_ bool isOffsetNegative = false
		);

		static void CopySinglePropertyRange(
			_In_ IDWriteTextLayout* oldLayout,
			_In_ unsigned int startPosForOld,
			_In_ IDWriteTextLayout* newLayout,
			_In_ unsigned int startPosForNew,
			_In_ unsigned int length,
			_In_opt_ CaretFormat* caretFormat = nullptr
		);

	private:
		IDWriteFactory* m_pFactory;
	};
}


