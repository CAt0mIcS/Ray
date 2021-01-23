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
		//glGenBuffers(1, &m_Buffer);
		//glBindBuffer(GL_UNIFORM_BUFFER, m_Buffer);
		//int blockIndex = glGetUniformBlockIndex(pShader->GetProgram(), "type_TriangleColor");
		//glUniformBlockBinding(pShader->GetProgram(), blockIndex, 1);
		//glBindBufferBase(GL_UNIFORM_BUFFER, 1, m_Buffer);
		//glBufferData(GL_UNIFORM_BUFFER, sizeof(color), color, GL_DYNAMIC_DRAW);
	}

	template<typename T>
	OpenGLConstantBuffer<T>::OpenGLConstantBuffer(std::string_view name, const Shader* pShader)
		: m_Shader(pShader), m_Name(name)
	{
		// Insert the prefix given to types when transcompiling shader with ShaderConductor
		m_Name.insert(0, "type_");
		//glGenBuffers(1, &m_Buffer);
	}

	template<typename T>
	void OpenGLConstantBuffer<T>::Update(const T& data)
	{
		m_Data = data;
	}

	template<typename T>
	void OpenGLPixelConstantBuffer<T>::Bind()
	{
		const OpenGLShader* pShader = (const OpenGLShader*)m_Shader;

		uint32_t buffer;
		glGenBuffers(1, &buffer);
		glBindBuffer(GL_UNIFORM_BUFFER, buffer);
		int blockIndex = glGetUniformBlockIndex(pShader->GetShaderProgram(), m_Name.c_str());
		glUniformBlockBinding(pShader->GetShaderProgram(), blockIndex, 1);
		glBindBufferBase(GL_UNIFORM_BUFFER, 1, buffer);
		glBufferData(GL_UNIFORM_BUFFER, sizeof(m_Data), &m_Data, GL_DYNAMIC_DRAW);
	}

	template<typename T>
	void OpenGLVertexConstantBuffer<T>::Bind()
	{
		const OpenGLShader* pShader = (const OpenGLShader*)m_Shader;

		glGenBuffers(1, &m_Buffer);
		glBindBuffer(GL_UNIFORM_BUFFER, m_Buffer);
		int blockIndex = glGetUniformBlockIndex(pShader->GetShaderProgram(), m_Name.c_str());
		glUniformBlockBinding(pShader->GetShaderProgram(), blockIndex, 1);
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