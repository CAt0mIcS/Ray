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


		void SaveScene(Application& app);
		void LoadScene(Application& app);
		void CreateDefaultTemplate(Application& app);

		void CreateDatabase(const std::string& filepath);


	private:
		QRD::Database* m_Db;
	};
}



