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
		// RAY_TODO: Apply vertex changes
		for (uint32_t i = 0; i < inputElements.size(); ++i)
		{
			glEnableVertexAttribArray(i);
			glVertexAttribPointer(i, 3, GL_FLOAT, GL_FALSE, 12, nullptr);
		}
	}

	void OpenGLInputLayout::Bind()
	{

	}
}




