#include "Rpch.h"
#include "RSkybox.h"

#include "Graphics/Images/RTexture2D.h"

#include <ktx.h>
#include <ktxvulkan.h>


namespace At0::Ray
{
	Skybox::Skybox(Entity entity) : Component(entity)
	{
		std::string filepath = "Resources/Textures/cubemap_space.ktx";

		ktxResult result;
		ktxTexture* ktxTexture;

		result = ktxTexture_CreateFromNamedFile(
			filepath.c_str(), KTX_TEXTURE_CREATE_LOAD_IMAGE_DATA_BIT, &ktxTexture);
		RAY_MEXPECTS(result == KTX_SUCCESS, "[Skybox] Failed to load texture \"{0}\"", filepath);
	}
}  // namespace At0::Ray
