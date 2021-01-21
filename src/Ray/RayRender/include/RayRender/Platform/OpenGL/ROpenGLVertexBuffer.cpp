#include "RayRender/RRpch.h"

#include "ROpenGLVertexBuffer.h"

#include <RayUtil/RException.h>
#include <RayDebug/RInstrumentor.h>

#include <glad/glad.h>


namespace At0::Ray
{
	static std::vector<Vertex> FlipVerticesToRightHandedCoordinateSystem(std::initializer_list<Vertex>&& data)
	{
		std::vector<Vertex> vertices = std::move(data);

		for (uint32_t i = 0; i < vertices.size() && i > vertices.size() - i; i += 2)
		{
			vertices.insert(vertices.begin() + i, { *(vertices.end() - i) });
			vertices.erase(vertices.end() - i);
		}

		return vertices;
	}

	OpenGLVertexBuffer::OpenGLVertexBuffer(std::initializer_list<Vertex> data)
	{
		RAY_PROFILE_FUNCTION();

		std::vector<Vertex> convertedData = FlipVerticesToRightHandedCoordinateSystem(std::move(data));

		glGenBuffers(1, &m_Buffer);
		glBindBuffer(GL_ARRAY_BUFFER, m_Buffer);
		glBufferData(GL_ARRAY_BUFFER, convertedData.size() * sizeof(Vertex), convertedData.data(), GL_STATIC_DRAW);
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



