#pragma once

#include <QRD/QRD.h>

#include <GUI/GUIApplication.h>


namespace NPE
{
	class Application : public GUI::GUIApplication
	{
	public:
		/**
		* Application constructor
		*/
		Application();

		/**
		* Receives all events
		* 
		* @param watched is the control that received the event
		* @param e is the received event
		*/
		bool OnEvent(GUI::Control* watched, GUI::Event& e);

		/**
		* Application deconstructor, saves scene to file
		*/
		~Application();

	private:
		QRD::Database m_Database;
	};
}



