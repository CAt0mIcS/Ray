#include "RScriptableEntity.h"


namespace At0::Ray
{
	ScriptableEntity::ScriptableEntity(
		Entity entity, std::string_view filepath, std::string_view className)
		: Component(entity)
	{
		if (std::filesystem::path(filepath).extension() == "dll")
			m_Script = Mono::Script::FromCompiled(filepath);
		else
			m_Script = Mono::Script::FromFile(filepath);

		m_Object = m_Script.GetObject(className);
	}
}  // namespace At0::Ray
