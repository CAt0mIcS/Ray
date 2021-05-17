#pragma once

#include "../RBase.h"
#include "RFunctionBase.h"


namespace At0::Ray::Mono
{
	class RAY_EXPORT Function : public FunctionBase
	{
		/**
		 * @param functionDescriptor Mono-compatible function descriptor
		 * @param pObject Object which should call the function
		 */
	public:
		Function(std::string_view methodName, MonoObject* pObject);

		/**
		 * Invokes the method with the parameters
		 * @param ppParameters List of all parameters of the function
		 */
		virtual void Invoke(void** ppParamters) const override;

	private:
		MonoMethod* m_Method = nullptr;
		MonoObject* m_Object = nullptr;
	};
}  // namespace At0::Ray::Mono
