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

		glCreateBuffers(1, &m_ElementBuffer);

		// GL_ELEMENT_ARRAY_BUFFER is not valid without an actively bound VAO
		// Binding with GL_ARRAY_BUFFER allows the data to be loaded regardless of VAO state. 
		glBindBuffer(GL_ARRAY_BUFFER, m_ElementBuffer);
		glBufferData(GL_ARRAY_BUFFER, m_NumIndices * sizeof(uint32_t), indices.begin(), GL_STATIC_DRAW);
	}

	OpenGLIndexBuffer::~OpenGLIndexBuffer()
	{
		glDeleteBuffers(1, &m_ElementBuffer);
	}

	void OpenGLIndexBuffer::Bind()
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ElementBuffer);
	}

	uint32_t OpenGLIndexBuffer::GetIndicesCount() const
	{
		return m_NumIndices;
	}
}


