#pragma once

#include "../RBase.h"

#include "RFunctionBase.h"


namespace At0::Ray::Mono
{
	class RAY_EXPORT StaticFunction : public FunctionBase
	{
	public:
		/**
		 * @param functionDescriptor Mono-compatible function descriptor
		 * @param pImage Image where the function is contained
		 */
		StaticFunction(std::string_view functionDescriptor, MonoImage* pImage);
		StaticFunction() = default;

		/**
		 * Invokes the method with the parameters
		 * @param ppParameters List of all parameters of the function
		 */
		virtual void Invoke(void** ppParamters) const override;

	private:
		MonoImage* m_Image;
		MonoMethod* m_Method;
	};
}  // namespace At0::Ray::Mono
