#include "../../RRpch.h"

#include "ROpenGLIndexBuffer.h"

#include <RayUtil/RException.h>
#include <glad/glad.h>


namespace At0::Ray
{
	OpenGLIndexBuffer::OpenGLIndexBuffer(std::initializer_list<uint32_t> indices)
		: m_NumIndices(indices.size())
	{
		glGenBuffers(1, &m_ElementBuffer);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_NumIndices * sizeof(uint32_t), indices.begin(), GL_STATIC_DRAW);
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


