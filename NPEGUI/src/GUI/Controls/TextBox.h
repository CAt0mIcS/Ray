#pragma once

#include "Control.h"

#include "GUI/Graphics/TextRenderer.h"

/**
* QUESTION:
*	Should I add class declarations like this or just include the header?
*/


namespace GUI
{
	class Node;
	class MouseButtonPressedEvent;
	class SetCursorEvent;
	class MouseButtonReleasedEvent;
	class KeyPressedEvent;

	class GUI_API TextBox : public Control
	{
		friend class HWNDTextBox;
	private:
		class Caret
		{
		public:
			/**
			* Modes which define the behaviour of SetSelection
			*/
			enum class SetSelectionMode
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
			Caret(TextBox* parent);

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
			void SetSelection(SetSelectionMode moveMode, unsigned int advance, bool extendSelection, bool updateCaretFormat = true);

			/**
			* @param isTrailingHit is the bool received from HitTestPoint
			* @param skipZeroWidth is true if zero widht should be skipped
			*/
			void AlignCaretToNearestCluster(bool isTrailingHit = false, bool skipZeroWidth = false);

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

			void OnReturnPressed(KeyPressedEvent& e);
			void OnBackPressed(KeyPressedEvent& e);
			void OnLeftPressed(KeyPressedEvent& e);
			void OnRightPressed(KeyPressedEvent& e);
			void OnUpPressed(KeyPressedEvent& e);
			void OnDownPressed(KeyPressedEvent& e);

			void DeleteSelection();

			DWRITE_TEXT_RANGE GetSelectionRange();
			void GetLineFromPosition(const DWRITE_LINE_METRICS* lineMetrics, unsigned int lineCount, unsigned int textPosition, unsigned int* lineOut, unsigned int* linePositionOut);

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
			TextBox* m_Parent;
		};

	public:
		/**
		* TextBox constructor
		*
		* @param paarent is the parent control
		*/
		TextBox(Control* parent = nullptr);

		/**
		* Calls the renderer and draws the textbox
		*
		* @returns true if the textbox was rendered (in window), false otherwise
		* @warning function does not call BeginDraw/EndDraw
		*/
		virtual bool Render() override;

		/**
		* All events of the specific control will be dispatched to this function
		*
		* @param e is the received event
		* @returns true if the event was handled, else false and the event will be dispatched to the client
		*/
		virtual bool OnEvent(Event& e) override;

		/**
		* Renders caret to position in text
		*/
		void RenderCaret();

		/**
		* Renders text in member variable
		*
		* @see TextBox::SetText()
		*/
		void RenderText();

		/**
		* Getter for current text in textbox
		*
		* @returns the current text
		*/
		const NText& GetText() const { return m_Text; }

		/**
		* Setter for text displayed in the textbox
		*
		* @param newText is the new text for the textbox
		* @warning function does not render the text, it just sets the member variable
		* @see TextBox::RenderText()
		*/
		void SetText(const std::wstring& newText) { m_Text.text = newText; }

		/**
		* Setter for multiline textbox
		*
		* @param multiline should be true if the textbox should be multiline, false otherwise
		*/
		void SetMultiline(const bool multiline) { m_IsMultiline = multiline; }

		/**
		* Setter for the font family
		*
		* @param family is the new font family
		*/
		void SetFontFamily(const std::wstring& family) { m_Text.fontFamily = family; }

		/**
		* Setter for the font size
		*
		* @param size is the new font size
		*/
		void SetFontSize(const float size) { m_Text.fontSize = size; }

		/**
		* Getter for font size
		*
		* @returns the current font size of the text
		*/
		float GetFontSize() const { return m_Text.fontSize; }

		/**
		* Calculates the layout of a new textbox
		*
		* @param parentPos is the position of the parent control
		* @param parentSize is the size of the parent control
		* @returns the new position and size of the layout
		*/
		virtual std::optional<std::pair<Util::NPoint, Util::NSize>> CalculateLayout(const Util::NPoint& parentPos, const Util::NSize& parentSize) override;

	private:
		/**
		* Handles cursor changed events
		*
		* @param e is the cursor event
		*/
		void OnSetCursor(SetCursorEvent& e);

		/**
		* Handles left clicked mouse button press
		*
		* @param e is the received event
		*/
		void OnMouseButtonPressed(MouseButtonPressedEvent& e);

		/**
		* Handles left clicked mouse button release
		*
		* @param e is the received
		*/
		void OnMouseButtonReleased(MouseButtonReleasedEvent& e);

		/**
		* Handles key pressed events
		* 
		* @param e is the received event
		*/
		void OnKeyPressed(KeyPressedEvent& e);

	private:
		NText m_Text;

		Caret m_Caret;

		bool m_IsMultiline;
		bool m_CurrentlySelecting;

	};
}



