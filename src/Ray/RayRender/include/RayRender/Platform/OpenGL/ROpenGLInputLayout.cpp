#include "../../RRpch.h"

#include "ROpenGLInputLayout.h"
#include "ROpenGLVertexShader.h"

#include <RayUtil/RException.h>
#include <RayDebug/RAssert.h>

#include <glad/glad.h>
#include "../../RVertexBuffer.h"
#include "../../Core/RVertex.h"


namespace At0::Ray
{
	OpenGLInputLayout::OpenGLInputLayout(std::initializer_list<InputElement> inputElements, const VertexShader* vShader)
	{
	}

	void OpenGLInputLayout::Bind()
	{

	}
}




