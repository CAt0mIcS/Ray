#pragma once

#include "GUIBase.h"

#include "Win.h"
#include <dwrite.h>

#include "Util/Util.h"


namespace GUI
{
	class TextBox;
	class MouseButtonPressedEvent;
	class MouseButtonReleasedEvent;
	class KeyPressedEvent;
	class CharEvent;
	class MouseMoveEvent;

	class GUI_API Caret
	{
	public:
		/// <summary>
		/// Modes which define the behaviour of SetSelection
		/// </summary>
		enum class MoveMode
		{
			Right,
			Left,
			RightChar,
			LeftChar,
			Up,
			Down,
			LeftWord,
			RightWord,
			AbsoluteTrailing,
			AbsoluteLeading
		};
	public:
		/// <summary>
		/// Caret Constructor
		/// </summary>
		/// <param name="parent">Is the GUI::TextBox which owns the caret</param>
		Caret(_In_ TextBox* parent);

		/// <summary>
		/// Draws caret and selection
		/// </summary>
		void Render();

		/// <summary>
		/// Getter for current caret pos
		/// </summary>
		/// <returns>The current position of the caret</returns>
		unsigned int Pos() const { return m_CaretPos; }

		/// <summary>
		/// Getter for the current caret pos offset
		/// </summary>
		/// <returns>The current caret pos offset</returns>
		unsigned int PosOffset() const { return m_CaretPosOffset; }

		/// <summary>
		/// Sets the selection depending on the mode
		/// </summary>
		/// <param name="moveMode">Is the type of move which defines the behaviour of the function</param>
		/// <param name="advance">Is the new position of the caret</param>
		/// <param name="extendSelection">Is true when the user selected part of the text</param>
		/// <param name="updateCaretFormat">Is true if a new text format should be applied</param>
		void SetSelection(
			_In_ MoveMode moveMode,
			_In_ unsigned int advance, 
			_In_ bool extendSelection, 
			_In_opt_ bool updateCaretFormat = true
		);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="isTrailingHit">Is the bool received from HitTestPoint</param>
		/// <param name="skipZeroWidth">Is true if zero widht should be skipped</param>
		void AlignCaretToNearestCluster(
			_In_opt_ bool isTrailingHit = false, 
			_In_opt_ bool skipZeroWidth = false
		);

		/// <summary>
		/// Coppies all text properties from the char after the caret and sets the proper caret formaat
		/// </summary>
		void UpdateCaretFormatting();

		/// <summary>
		/// Calculates caret rect
		/// </summary>
		/// <returns>The caret pos and size relative to the text layout box</returns>
		D2D1_RECT_F GetCaretRect();

		/// <summary>
		/// Calculates the caret thickness
		/// </summary>
		/// <returns>The caret thickness</returns>
		float GetCaretThickness();

		/// <summary>
		/// Starts the selection from a specific point
		/// </summary>
		/// <param name="pos">Is the point where the selection will start</param>
		/// <param name="extendSelection">Is true if the selection should be extended</param>
		void SetSelectionFromPoint(
			_In_ const Util::NPoint& pos, 
			_In_ bool extendSelection
		);

		/// <summary>
		/// Handles mouse Button press events
		/// </summary>
		/// <param name="e">Is the received event</param>
		void OnMouseButtonPressed(
			_In_ MouseButtonPressedEvent& e
		);

		/// <summary>
		/// Handles mouse Button release events
		/// </summary>
		/// <param name="e">Is the received event</param>
		void OnMouseButtonReleased(
			_In_ MouseButtonReleasedEvent& e
		);

		/// <summary>
		/// Handles mouse move events
		/// </summary>
		/// <param name="e">Is the received event</param>
		void OnMouseMove(
			_In_ MouseMoveEvent& e
		);

		/// <summary>
		/// Handles char press events
		/// </summary>
		/// <param name="e">Is the received event</param>
		void OnCharEvent(
			_In_ CharEvent& e
		);

		/// <summary>
		/// Handles al necessary key presses events
		/// </summary>
		/// <param name="e">Is the received event</param>
		void OnKeyPressed(
			_In_ KeyPressedEvent& e
		);

		/// <summary>
		/// Calculates the line from a position
		/// </summary>
		/// <param name="lineMetrics">Are the metrics</param>
		/// <param name="lineCount">Is the amount of lines</param>
		/// <param name="textPosition">Is the position in the global text</param>
		/// <param name="lineOut">Is the output of line count</param>
		/// <param name="linePositionOut">Is the output of line position</param>
		void GetLineFromPosition(
			_In_ const DWRITE_LINE_METRICS* lineMetrics, 
			_In_ unsigned int lineCount, 
			_In_ unsigned int textPosition, 
			_Out_ unsigned int* lineOut, 
			_Out_ unsigned int* linePositionOut
		);

		/// <summary>
		/// Deletes the current selection
		/// </summary>
		void DeleteSelection();

		/// <summary>
		/// Calculates the selection in the text
		/// </summary>
		/// <returns>The current selection range</returns>
		DWRITE_TEXT_RANGE GetSelectionRange();

	private:
		/// <summary>
		/// Renders caret position in the text
		/// </summary>
		void RenderCaret();

		/// <summary>
		/// Draws selection rect
		/// </summary>
		void RenderSelection();

	private:
		/// <summary>
		/// The important range based properties for the current caret.
		/// Note these are stored outside the layout, since the current car
		/// actually has a format, independent of the text it lies between.
		/// </summary>
		struct CaretFormat
		{
			wchar_t fontFamilyName[100];
			wchar_t localeName[LOCALE_NAME_MAX_LENGTH];
			FLOAT fontSize;
			DWRITE_FONT_WEIGHT fontWeight;
			DWRITE_FONT_STRETCH fontStretch;
			DWRITE_FONT_STYLE fontStyle;
			UINT32 color;
			BOOL hasUnderline;
			BOOL hasStrikethrough;
		};

		CaretFormat m_CaretFormat;
		unsigned int m_CaretPos;
		unsigned int m_CaretPosOffset;
		unsigned int m_CaretAnchor;

		bool m_CurrentlySelecting;

		TextBox* m_Parent;
	};
}



