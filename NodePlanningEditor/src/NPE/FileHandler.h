#pragma once

#include <string>
#include <QRD/QRD.h>


namespace NPE
{
	class Application;

	class FileHandler
	{
	public:
		FileHandler() = default;


		void SaveScene(Application& app, int zoom);
		void LoadScene(Application& app, int& zoom);
		void CreateDefaultTemplate(int zoom);

		void CreateDatabase(const std::string& filepath);


	private:
		QRD::Database* m_Db;
	};
}



