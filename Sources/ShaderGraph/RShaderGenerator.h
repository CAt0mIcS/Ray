#pragma once

#include "../RBase.h"
#include "../Utils/RNonCopyable.h"

#include "Techniques/RTechnique.h"

#include <unordered_map>


namespace At0::Ray
{
	class RAY_EXPORT ShaderGenerator : public Technique
	{
	public:
		/**
		 * Generates the shaders
		 */
		virtual std::vector<std::string> Generate() const = 0;

	protected:
		ShaderGenerator() = default;

		/**
		 * Returns the default shader code, ready to insert into.
		 * VertexShader:
		 *	{0}: Input attributes
		 *	{1}: Output attributes
		 *	{2}: Extra per-scene data (default: View, Proj)
		 *	{3}: Extra per-object data (default: Model)
		 *	{4}: Extra uniforms (default: PerScene, PerObject)
		 *	{5}: Variables
		 *	{6}: Functions
		 *	{7}: Main shader code
		 *
		 * FragmentShader:
		 *	{0}: Input attributes
		 *	{1}: Uniforms
		 *	{2}: Variables
		 *	{3}: Functions
		 *	{4}: Main shader code
		 */
		std::vector<std::string> GetShaderTemplates() const;
	};
}  // namespace At0::Ray
