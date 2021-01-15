#pragma once

#include "RMath.h"

#include <string>


namespace At0::Ray
{
	struct TagComponent
	{
		std::string Tag;

		TagComponent()
			: Tag("New Entity") {}

		TagComponent(std::string_view tag)
			: Tag(tag) {}
	};

	struct TransformComponent
	{
		Float3 Scale;
		Float3 Translation;
		Float3 Rotation;

		TransformComponent()
			: Scale{ 1.0f, 1.0f, 1.0f }, Translation{ 0.0f, 0.0f, 0.0f }, Rotation{ } {}

		TransformComponent(const Float3& scale, const Float3& translation, const Float3& rotation)
			: Scale(scale), Translation(translation), Rotation(rotation) {}

		Ray::Matrix ToMatrix() const
		{
			return Ray::Matrix::Scaling(Scale.x, Scale.y, Scale.z) *
				Ray::Matrix::Translation(Translation.x, Translation.y, Translation.z) *
				Ray::Matrix::RotationRollPitchYawFromVector({ Rotation.x, Rotation.y, Rotation.z, 0.0f });
		}
	};

	class VertexShader;
	struct VertexShaderComponent
	{
		Ref<VertexShader> Shader;

		VertexShaderComponent(Ref<VertexShader> shader)
			: Shader(shader) {}

		VertexShaderComponent()
			: Shader(nullptr) {}
	};

	class PixelShader;
	struct PixelShaderComponent
	{
		Ref<PixelShader> Shader;

		PixelShaderComponent(Ref<PixelShader> shader)
			: Shader(shader) {}

		PixelShaderComponent()
			: Shader(nullptr) {}
	};

	class InputLayout;
	struct InputLayoutComponent
	{
		Ref<InputLayout> Layout;

		InputLayoutComponent(Ref<InputLayout> layout)
			: Layout(layout) {}

		InputLayoutComponent()
			: Layout(nullptr) {}
	};

	class IndexBuffer;
	struct IndexBufferComponent
	{
		Ref<IndexBuffer> Buffer;

		IndexBufferComponent(Ref<IndexBuffer> buffer)
			: Buffer(buffer) {}

		IndexBufferComponent()
			: Buffer(nullptr) {}
	};
}