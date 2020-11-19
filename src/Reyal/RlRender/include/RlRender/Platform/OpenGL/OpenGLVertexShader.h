#ifndef RLR_OPENGLVERTEXSHADER
#define RLR_OPENGLVERTEXSHADER

#include "RlRBase.h"
#include "RlRender/VertexShader.h"

#include <string>


namespace At0::Reyal
{
	class RLR_API OpenGLVertexShader : public VertexShader
	{
	public:
		OpenGLVertexShader(const std::string_view filepath);
	};
}



#endif // RLR_OPENGLVERTEXSHADER