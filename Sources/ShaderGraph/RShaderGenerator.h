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
		virtual std::vector<std::string> Generate() const = 0;

		template<typename T = Technique>
		T& GetTechnique(Connection connectionPoint)
		{
			return *(T*)m_Connections.at(connectionPoint).get();
		}

		template<typename T = Technique>
		const T& GetTechnique(Connection connectionPoint) const
		{
			return *(const T*)m_Connections.at(connectionPoint).get();
		}

	protected:
		/**
		 * Returns the default shader code, ready to insert into.
		 * VertexShader:
		 *	{0}: Input attributes
		 *	{1}: Output attributes
		 *	{2}: Extra per-scene data (default: View, Proj)
		 *	{3}: Extra per-object data (default: Model)
		 *	{4}: Extra uniforms (default: PerScene, PerObject)
		 *	{5}: Functions
		 *	{6}: Main shader code
		 *
		 * FragmentShader:
		 *	{0}: Input attributes
		 *	{1}: Uniforms
		 *	{2}: Functions
		 *	{3}: Main shader code
		 */
		std::vector<std::string> GetShaderTemplates() const;

	protected:
		ShaderGenerator() = default;

	protected:
		std::unordered_map<Connection, Scope<Technique>> m_Connections;
	};
}  // namespace At0::Ray
