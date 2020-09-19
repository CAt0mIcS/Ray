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
		/**
		* Modes which define the behaviour of SetSelection
		*/
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
		/**
		* Caret constructor
		*
		* @param parent is the textbox which owns the caret
		*/
		Caret(_In_ TextBox* parent);

		/**
		* Draws caret and selection
		*/
		void Render();

		/**
		* Getter for current caret position
		*
		* @returns the current caret position
		*/
		unsigned int Pos() const { return m_CaretPos; }

		/**
		* Getter for current caret position offset
		*
		* @returns the current caret position offset
		*/
		unsigned int PosOffset() const { return m_CaretPosOffset; }

		/**
		* Sets the selection depending on the mode
		*
		* @param moveMode is the type of move which defines the behaviour of the function
		* @param advance is the new position of the caret
		* @param extendSelection is true when the user selected part of the text
		*/
		void SetSelection(
			_In_ MoveMode moveMode,
			_In_ unsigned int advance, 
			_In_ bool extendSelection, 
			_In_opt_ bool updateCaretFormat = true
		);

		/**
		* @param isTrailingHit is the bool received from HitTestPoint
		* @param skipZeroWidth is true if zero widht should be skipped
		*/
		void AlignCaretToNearestCluster(
			_In_opt_ bool isTrailingHit = false, 
			_In_opt_ bool skipZeroWidth = false
		);

		/**
		* Coppies all text properties from the char after the caret and sets the proper caret formaat
		*/
		void UpdateCaretFormatting();

		/**
		* Calculate caret rect
		*
		* @returns the caret pos and size relative to the text layout box
		*/
		D2D1_RECT_F GetCaretRect();

		/**
		* Calculates the caret thickness
		*
		* @returns the caret thickness
		*/
		float GetCaretThickness();

		/**
		* Starts the selection from a specific point
		* 
		* @param pos is the point where the selection will start
		* @param extendSelection is true if the selection should be extended
		*/
		void SetSelectionFromPoint(
			_In_ const Util::NPoint& pos, 
			_In_ bool extendSelection
		);

		/**
		* Handles mouse button pressed events
		*
		* @param e is the received event
		*/
		void OnMouseButtonPressed(
			_In_ MouseButtonPressedEvent& e
		);

		/**
		* Handles mouse button released events
		* 
		* @param e is the received event
		*/
		void OnMouseButtonReleased(
			_In_ MouseButtonReleasedEvent& e
		);

		/**
		* Handles mouse move events
		* 
		* @param e is the received event
		*/
		void OnMouseMove(
			_In_ MouseMoveEvent& e
		);

		/**
		* Handles character events
		*
		* @param e is the received event
		*/
		void OnCharEvent(
			_In_ CharEvent& e
		);

		/**
		* Handles all necessary key presses
		*
		* @param e is the received event
		*/
		void OnKeyPressed(
			_In_ KeyPressedEvent& e
		);

		/**
		* Calculates the line from a position
		*
		* @param lineMetrics are the metrics
		* @param lineCount is the amount of lines
		* @param textPosition is the position in the global text
		* @param lineOut is the output of line count
		* @param linePositionOut is the output of line position
		*/
		void GetLineFromPosition(
			_In_ const DWRITE_LINE_METRICS* lineMetrics, 
			_In_ unsigned int lineCount, 
			_In_ unsigned int textPosition, 
			_Out_ unsigned int* lineOut, 
			_Out_ unsigned int* linePositionOut
		);

		/**
		* Deletes the current selection
		*/
		void DeleteSelection();

		/**
		* Calculates the selection in the text
		*
		* @returns the selection range
		*/
		DWRITE_TEXT_RANGE GetSelectionRange();

	private:
		/**
		* Renders caret to position in text
		*/
		void RenderCaret();

		/**
		* Draws the selection rect
		*/
		void RenderSelection();

	private:
		/**
		* The important range based properties for the current caret.
		* Note these are stored outside the layout, since the current caret
		* actually has a format, independent of the text it lies between.
		*/
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



