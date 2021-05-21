#pragma once

#include "../RBase.h"
#include "../Utils/RNonCopyable.h"

#include "Techniques/RTechnique.h"

#include <unordered_map>


namespace At0::Ray
{
	class RAY_EXPORT ShaderGenerator : NonCopyable
	{
	public:
		enum class Connection
		{
			Color
		};

	public:
		virtual ~ShaderGenerator() = default;

		/**
		 * Connects a point on the main shader node to another subnode(technique)
		 */
		void Connect(Connection connectionPoint, Scope<Technique> technique);

		/**
		 * @returns What is connected to the main shader nodes
		 */
		const auto& GetConnections() const { return m_Connections; }

		/**
		 * Generates the shaders
		 */
		std::vector<std::string> Generate() const;

		template<typename T = Technique>
		T& GetTechnique(Connection connectionPoint)
		{
			return *(T*)m_Connections[connectionPoint].get();
		}

		template<typename T = Technique>
		const T& GetTechnique(Connection connectionPoint) const
		{
			return *(const T*)m_Connections[connectionPoint].get();
		}

	protected:
		ShaderGenerator() = default;

	protected:
		std::unordered_map<Connection, Scope<Technique>> m_Connections;
	};
}  // namespace At0::Ray
