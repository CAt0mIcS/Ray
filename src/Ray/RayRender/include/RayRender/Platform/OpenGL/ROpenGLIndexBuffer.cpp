#include "../../RRpch.h"

#include "ROpenGLIndexBuffer.h"

#include <RayUtil/RException.h>
#include <RayDebug/RInstrumentor.h>
#include <glad/glad.h>


namespace At0::Ray
{
	OpenGLIndexBuffer::OpenGLIndexBuffer(std::initializer_list<uint32_t> indices)
		: m_NumIndices(indices.size())
	{
		RAY_PROFILE_FUNCTION();

		glGenBuffers(1, &m_Buffer);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Buffer);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(uint32_t), indices.begin(), GL_STATIC_DRAW);
	}

	OpenGLIndexBuffer::~OpenGLIndexBuffer()
	{
		glDeleteBuffers(1, &m_Buffer);
	}

	void OpenGLIndexBuffer::Bind()
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Buffer);
	}

	uint32_t OpenGLIndexBuffer::GetIndicesCount() const
	{
		return m_NumIndices;
	}
}


