#pragma once

#include "../RlRBase.h"

struct aiScene;
struct aiMesh;
struct aiNode;


namespace At0::Ray
{
	class RR_API Mesh
	{
	public:
		Mesh(std::string_view filepath);

		void Draw();

	private:

	private:

	};
}