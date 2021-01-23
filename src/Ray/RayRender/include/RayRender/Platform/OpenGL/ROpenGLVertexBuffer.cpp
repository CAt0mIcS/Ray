#include "RayRender/RRpch.h"

#include "ROpenGLVertexBuffer.h"
#include "ROpenGLShader.h"
#include "../../Core/RVertex.h"

#include <RayUtil/RException.h>
#include <RayDebug/RInstrumentor.h>

#include <glad/glad.h>


namespace At0::Ray
{
	static VertexData FlipVerticesToRightHandedCoordinateSystem(VertexData data)
	{
		RAY_PROFILE_FUNCTION();

		for (uint32_t i = 0; i < data.Size() && i > data.Size() - i; i += 2)
		{
			data.Swap(i, data.Size() - i);
		}

		return data;
	}

	OpenGLVertexBuffer::OpenGLVertexBuffer(const VertexData& data, const Scope<Shader>& vShader)
	{
		RAY_PROFILE_FUNCTION();

		VertexData convertedData = FlipVerticesToRightHandedCoordinateSystem(data);

		glGenBuffers(1, &m_Buffer);
		glBindBuffer(GL_ARRAY_BUFFER, m_Buffer);
		glBufferData(GL_ARRAY_BUFFER, convertedData.SizeBytes(), convertedData.Data(), GL_STATIC_DRAW);

		for (uint32_t i = 0; i < convertedData.NumberOfElementTypes(); ++i)
		{
			glEnableVertexAttribArray(i);
			glVertexAttribPointer(i,
				convertedData.Layout().NumberOfComponentsInType(convertedData.Layout()[i].Type()),
				GL_FLOAT, GL_FALSE, 0, nullptr
			);
		}
	}

	OpenGLVertexBuffer::~OpenGLVertexBuffer()
	{
		glDeleteBuffers(1, &m_Buffer);
	}

	void OpenGLVertexBuffer::Bind()
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_Buffer);
	}
}



