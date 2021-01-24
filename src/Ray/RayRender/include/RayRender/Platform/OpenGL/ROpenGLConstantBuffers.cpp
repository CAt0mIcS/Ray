#include "../../RRpch.h"
#include "ROpenGLConstantBuffers.h"

#include "ROpenGLShader.h"

#include <glad/glad.h>


namespace At0::Ray
{
	template<typename T>
	OpenGLConstantBuffer<T>::OpenGLConstantBuffer(std::string_view name, const Shader* pShader, const T& data)
		: m_Shader(pShader), m_Name(name)
	{
		// Insert the prefix given to types when transcompiling shader with ShaderConductor
		m_Name.insert(0, "type_");

		glGenBuffers(1, &m_Buffer);
		glBindBuffer(GL_UNIFORM_BUFFER, m_Buffer);
		int blockIndex = glGetUniformBlockIndex(((OpenGLShader*)pShader)->GetShaderProgram(), m_Name.c_str());
		glUniformBlockBinding(((OpenGLShader*)pShader)->GetShaderProgram(), blockIndex, 1);

		glBindBufferBase(GL_UNIFORM_BUFFER, 1, m_Buffer);
		glBufferData(GL_UNIFORM_BUFFER, sizeof(data), &data, GL_DYNAMIC_DRAW);

		glBindBuffer(GL_UNIFORM_BUFFER, 0);
	}

	template<typename T>
	OpenGLConstantBuffer<T>::OpenGLConstantBuffer(std::string_view name, const Shader* pShader)
		: m_Shader(pShader), m_Name(name)
	{
		// Insert the prefix given to types when transcompiling shader with ShaderConductor
		m_Name.insert(0, "type_");

		glGenBuffers(1, &m_Buffer);
		glBindBuffer(GL_UNIFORM_BUFFER, m_Buffer);
		int blockIndex = glGetUniformBlockIndex(((OpenGLShader*)pShader)->GetShaderProgram(), m_Name.c_str());
		glUniformBlockBinding(((OpenGLShader*)pShader)->GetShaderProgram(), blockIndex, 1);

		glBindBuffer(GL_UNIFORM_BUFFER, 0);
	}

	template<typename T>
	void OpenGLConstantBuffer<T>::Update(const T& data)
	{
		glBindBuffer(GL_UNIFORM_BUFFER, m_Buffer);
		glBufferData(GL_UNIFORM_BUFFER, sizeof(data), &data, GL_DYNAMIC_DRAW);
		glBindBuffer(GL_UNIFORM_BUFFER, 0);
	}

	template<typename T>
	OpenGLConstantBuffer<T>::~OpenGLConstantBuffer()
	{
		glDeleteBuffers(1, &m_Buffer);
	}

	template<typename T>
	void OpenGLPixelConstantBuffer<T>::Bind()
	{
		const OpenGLShader* pShader = (const OpenGLShader*)m_Shader;

		glBindBuffer(GL_UNIFORM_BUFFER, m_Buffer);
		glBindBufferBase(GL_UNIFORM_BUFFER, 1, m_Buffer);
	}

	template<typename T>
	void OpenGLVertexConstantBuffer<T>::Bind()
	{
		const OpenGLShader* pShader = (const OpenGLShader*)m_Shader;

		glBindBuffer(GL_UNIFORM_BUFFER, m_Buffer);
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