#pragma once

#include "GUIBase.h"


namespace GUI
{
	/// <summary>
	/// Timer class, sets a Win32API timer
	/// </summary>
	class GUI_API Timer
	{
	public:
		/// <summary>
		/// Timer Constructor
		/// </summary>
		/// <param name="hWnd">Is a handle to the window which should own the Timer</param>
		/// <param name="repeat">Is true if the timer should not be destroyed after it finnished for the first time</param>
		Timer(_In_ HWND hWnd, _In_ bool repeat);

		/// <summary>
		/// Starts the Timer
		/// </summary>
		/// <param name="time">Is the time the Timer should run</param>
		void Run(_In_ unsigned int time);

		/// <summary>
		/// Getter for window handle of Timer
		/// </summary>
		/// <returns></returns>
		HWND GetWindow() const { return m_hWnd; }

		/// <summary>
		/// Checks if this Timer is repeating
		/// </summary>
		/// <returns>True if this Timer is repeating, false otherwise</returns>
		bool IsRepeating() const { return m_Repeat; }

		/// <summary>
		/// Getter for the Timer Id
		/// </summary>
		/// <returns>The Timer Id</returns>
		unsigned int GetId() const { return m_Id; }

		/// <summary>
		/// Destroys the Timer
		/// </summary>
		/// <returns>True if the Timer was destroyed successfully, false otherwise</returns>
		bool Destroy();

	private:
		/// <summary>
		/// Window handle, which window owns the Timer
		/// </summary>
		HWND m_hWnd;

		/// <summary>
		/// True if Timer should repeat
		/// </summary>
		bool m_Repeat;

		/// <summary>
		/// Id of this Timer
		/// </summary>
		unsigned int m_Id;

		/// <summary>
		/// Used to assign m_Id when creating a new Timer,
		/// variable is incremented in the Timer Constructor
		/// </summary>
		static unsigned int s_NextId;
	};
}



