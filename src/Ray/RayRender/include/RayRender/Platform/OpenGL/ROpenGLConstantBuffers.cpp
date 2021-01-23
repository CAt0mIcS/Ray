#include "../../RRpch.h"
#include "ROpenGLConstantBuffers.h"

#include "ROpenGLVertexShader.h"
#include "ROpenGLPixelShader.h"

#include <glad/glad.h>


namespace At0::Ray
{
	template<typename T>
	OpenGLConstantBuffer<T>::OpenGLConstantBuffer(const T& data)
	{
		//glGenBuffers(1, &m_Buffer);
		//glBindBuffer(GL_UNIFORM_BUFFER, m_Buffer);
		//int blockIndex = glGetUniformBlockIndex(pShader->GetProgram(), "type_TriangleColor");
		//glUniformBlockBinding(pShader->GetProgram(), blockIndex, 1);
		//glBindBufferBase(GL_UNIFORM_BUFFER, 1, m_Buffer);
		//glBufferData(GL_UNIFORM_BUFFER, sizeof(color), color, GL_DYNAMIC_DRAW);
	}

	template<typename T>
	OpenGLConstantBuffer<T>::OpenGLConstantBuffer()
	{
		//glGenBuffers(1, &m_Buffer);
	}

	template<typename T>
	void OpenGLConstantBuffer<T>::Update(const T& data)
	{
		m_Data = data;
	}


	template<typename T>
	OpenGLPixelConstantBuffer<T>::OpenGLPixelConstantBuffer(std::string_view name, const PixelShader* pShader, const T& data)
		: m_Name(name), m_PShader(pShader)
	{
		m_Name.insert(0, "type_");
	}

	template<typename T>
	OpenGLPixelConstantBuffer<T>::OpenGLPixelConstantBuffer(std::string_view name, const PixelShader* pShader)
		: m_Name(name), m_PShader(pShader)
	{
		m_Name.insert(0, "type_");
	}

	template<typename T>
	OpenGLVertexConstantBuffer<T>::OpenGLVertexConstantBuffer(std::string_view name, const VertexShader* pShader, const T& data)
		: m_Name(name), m_VShader(pShader)
	{
		m_Name.insert(0, "type_");
	}

	template<typename T>
	OpenGLVertexConstantBuffer<T>::OpenGLVertexConstantBuffer(std::string_view name, const VertexShader* pShader)
		: m_Name(name), m_VShader(pShader)
	{
		m_Name.insert(0, "type_");
	}

	template<typename T>
	void OpenGLPixelConstantBuffer<T>::Bind()
	{
		OpenGLPixelShader* pShader = (OpenGLPixelShader*)m_PShader;

		uint32_t buffer;
		glGenBuffers(1, &buffer);
		glBindBuffer(GL_UNIFORM_BUFFER, buffer);
		int blockIndex = glGetUniformBlockIndex(pShader->GetProgram(), m_Name.c_str());
		glUniformBlockBinding(pShader->GetProgram(), blockIndex, 1);
		glBindBufferBase(GL_UNIFORM_BUFFER, 1, buffer);
		glBufferData(GL_UNIFORM_BUFFER, sizeof(m_Data), &m_Data, GL_DYNAMIC_DRAW);
	}

	template<typename T>
	void OpenGLVertexConstantBuffer<T>::Bind()
	{
		OpenGLVertexShader* pShader = (OpenGLVertexShader*)m_VShader;

		glGenBuffers(1, &m_Buffer);
		glBindBuffer(GL_UNIFORM_BUFFER, m_Buffer);
		int blockIndex = glGetUniformBlockIndex(pShader->GetProgram(), m_Name.c_str());
		glUniformBlockBinding(pShader->GetProgram(), blockIndex, 1);
		glBindBufferBase(GL_UNIFORM_BUFFER, 1, m_Buffer);
		glBufferData(GL_UNIFORM_BUFFER, sizeof(m_Data), &m_Data, GL_DYNAMIC_DRAW);
	}


	// explicit instantiation
	template class OpenGLConstantBuffer<Matrix>;
	template class OpenGLConstantBuffer<Float3>;

	template class OpenGLPixelConstantBuffer<Matrix>;
	template class OpenGLPixelConstantBuffer<Float3>;

	template class OpenGLVertexConstantBuffer<Matrix>;
	template class OpenGLVertexConstantBuffer<Float3>;
}