#ifndef RLR_SHADER_H
#define RLR_SHADER_H

#include "RlRBase.h"
#include <../../RlUtilities/include/RlUtil/Helper.h>

#include <string>


namespace At0::Reyal
{
	class RLR_API VertexShader
	{
	public:
		/// <summary>
		/// Creates the RendererAPI dependent VertexShader
		/// </summary>
		/// <param name="filepath">Is the path to the file containing the vertex source</param>
		/// <returns>The created Shader</returns>
		static Ref<VertexShader> Create(const std::string_view filepath);

	protected:
		/// <summary>
		/// Default VertexShader Constructor
		/// </summary>
		VertexShader() = default;
	};
}


#endif // RLR_SHADER_H