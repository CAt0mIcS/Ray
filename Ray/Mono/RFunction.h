#pragma once

#include "../RBase.h"
#include "RFunctionBase.h"


namespace At0::Ray::Mono
{
	class RAY_EXPORT Function : public FunctionBase
	{
	public:
		Function(std::string_view methodName, MonoObject* pObject);
		Function() = default;

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
