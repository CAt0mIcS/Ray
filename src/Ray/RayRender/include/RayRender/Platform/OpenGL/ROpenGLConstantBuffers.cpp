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
		glGenBuffers(1, &m_Buffer);
		//glBindBuffer(GL_UNIFORM_BUFFER, m_Buffer);
		//int blockIndex = glGetUniformBlockIndex(pShader->GetProgram(), "type_TriangleColor");
		//glUniformBlockBinding(pShader->GetProgram(), blockIndex, 1);
		//glBindBufferBase(GL_UNIFORM_BUFFER, 1, m_Buffer);
		//glBufferData(GL_UNIFORM_BUFFER, sizeof(color), color, GL_DYNAMIC_DRAW);
	}

	template<typename T>
	OpenGLConstantBuffer<T>::OpenGLConstantBuffer()
	{
		glGenBuffers(1, &m_Buffer);
	}

	template<typename T>
	void OpenGLConstantBuffer<T>::Update(const T& data)
	{
		glBindBuffer(GL_UNIFORM_BUFFER, m_Buffer);
		glBufferData(GL_UNIFORM_BUFFER, sizeof(data), (void*)&data, GL_DYNAMIC_DRAW);
	}


	template<typename T>
	OpenGLPixelConstantBuffer<T>::OpenGLPixelConstantBuffer(std::string_view name, const PixelShader* pShader, const T& data)
		: m_Name(name), m_PShader(pShader)
	{
	}

	template<typename T>
	OpenGLPixelConstantBuffer<T>::OpenGLPixelConstantBuffer(std::string_view name, const PixelShader* pShader)
		: m_Name(name), m_PShader(pShader)
	{
	}

	template<typename T>
	OpenGLVertexConstantBuffer<T>::OpenGLVertexConstantBuffer(std::string_view name, const VertexShader* pShader, const T& data)
		: m_Name(name), m_VShader(pShader)
	{
	}

	template<typename T>
	OpenGLVertexConstantBuffer<T>::OpenGLVertexConstantBuffer(std::string_view name, const VertexShader* pShader)
		: m_Name(name), m_VShader(pShader)
	{
	}

	template<typename T>
	void OpenGLPixelConstantBuffer<T>::Bind()
	{
		OpenGLPixelShader* shader = (OpenGLPixelShader*)m_PShader;

		glBindBuffer(GL_UNIFORM_BUFFER, m_Buffer);
		int blockIndex = glGetUniformBlockIndex(shader->GetProgram(), m_Name.c_str());
		glUniformBlockBinding(shader->GetProgram(), blockIndex, 1);
		glBindBufferBase(GL_UNIFORM_BUFFER, 1, m_Buffer);
	}

	template<typename T>
	void OpenGLVertexConstantBuffer<T>::Bind()
	{
		OpenGLVertexShader* shader = (OpenGLVertexShader*)m_VShader;

		glBindBuffer(GL_UNIFORM_BUFFER, m_Buffer);
		int blockIndex = glGetUniformBlockIndex(shader->GetProgram(), m_Name.c_str());
		glUniformBlockBinding(shader->GetProgram(), blockIndex, 1);
		glBindBufferBase(GL_UNIFORM_BUFFER, 1, m_Buffer);
	}


	// explicit instantiation
	template class OpenGLConstantBuffer<Matrix>;
	template class OpenGLConstantBuffer<Float3>;

	template class OpenGLPixelConstantBuffer<Matrix>;
	template class OpenGLPixelConstantBuffer<Float3>;

	template class OpenGLVertexConstantBuffer<Matrix>;
	template class OpenGLVertexConstantBuffer<Float3>;
}