#include "RayRender/RRpch.h"

#include "ROpenGLVertexBuffer.h"
#include "../../Core/RVertex.h"

#include <RayUtil/RException.h>
#include <RayDebug/RInstrumentor.h>

#include <glad/glad.h>


namespace At0::Ray
{
	static VertexData FlipVerticesToRightHandedCoordinateSystem(const VertexData& data)
	{
		RAY_PROFILE_FUNCTION();

		// RAY_TODO: Performance: possible vector reallocation
		//std::vector<Vertex> vertices(data.Size());
		VertexData newData = data;

		for (uint32_t i = 0; i < newData.Size() && i > newData.Size() - i; i += 2)
		{
			//vertices.insert(vertices.begin() + i, { *(vertices.end() - i) });
			//vertices.erase(vertices.end() - i);

			newData.Swap(i, newData.Size() - i);
		}

		return newData;
	}

	OpenGLVertexBuffer::OpenGLVertexBuffer(const VertexData& data)
	{
		RAY_PROFILE_FUNCTION();

		VertexData convertedData = FlipVerticesToRightHandedCoordinateSystem(data);

		glGenBuffers(1, &m_Buffer);
		glBindBuffer(GL_ARRAY_BUFFER, m_Buffer);
		glBufferData(GL_ARRAY_BUFFER, convertedData.SizeBytes(), convertedData.Data(), GL_STATIC_DRAW);
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



