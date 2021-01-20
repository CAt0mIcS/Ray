float4 main(uint tid : SV_PrimitiveID) : SV_Target
{
	float4 face_colors[6] =
	{
		{ 1.0f, 1.0f, 1.0f, 1.0f },
		{ 1.0f, 1.0f, 0.0f, 1.0f },
		{ 1.0f, 0.0f, 1.0f, 1.0f },
		{ 0.0f, 1.0f, 1.0f, 1.0f },
		{ 1.0f, 0.0f, 0.0f, 1.0f },
		{ 0.0f, 1.0f, 0.0f, 1.0f }
	};

	return face_colors[(tid / 2) % 6];
}