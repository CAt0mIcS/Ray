#include "RayRender/RRpch.h"

#include "ROpenGLVertexBuffer.h"

#include <RayUtil/RException.h>
#include <RayDebug/RInstrumentor.h>

#include <glad/glad.h>


namespace At0::Ray
{
	OpenGLVertexBuffer::OpenGLVertexBuffer(std::initializer_list<Vertex> data)
	{
		RAY_PROFILE_FUNCTION();

		//glCreateBuffers(1, &m_Buffer);
		//glBindBuffer(GL_ARRAY_BUFFER, m_Buffer);
		//glBufferData(GL_ARRAY_BUFFER, data.size(), data.begin(), GL_STATIC_DRAW);
		// RAY_TODO: Might need to be GL_DYNAMIC_DRAW

		glGenBuffers(1, &m_Buffer);
		glBindBuffer(GL_ARRAY_BUFFER, m_Buffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof(data.begin()), data.begin(), GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), nullptr);
		glEnableVertexAttribArray(0);

		unsigned int VAO;
		glGenVertexArrays(1, &VAO);
		glBindVertexArray(VAO);
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



