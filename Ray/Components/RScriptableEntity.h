#pragma once

#include "../RBase.h"

#include "RComponent.h"
#include "../Mono/RScript.h"


namespace At0::Ray
{
	class RAY_EXPORT ScriptableEntity : public Component
	{
	public:
		/**
		 * @param filepath Path to the assembly. If the extension is .dll the component will
		 * interpret it as compiled. All other extensions will be interpreted as source code
		 * @param className Name of the class for which an object will be created. Only one class
		 * per component at the moment
		 */
		ScriptableEntity(Entity entity, std::string_view filepath, std::string_view className);

		const Mono::Script& GetScript() const { return m_Script; }
		const Mono::Object& GetObject() const { return m_Object; }
		Mono::Script& GetScript() { return m_Script; }
		Mono::Object& GetObject() { return m_Object; }

	private:
		Mono::Script m_Script;
		Mono::Object m_Object;
	};
}  // namespace At0::Ray


RAY_EXPORT_COMPONENT(ScriptableEntity);
