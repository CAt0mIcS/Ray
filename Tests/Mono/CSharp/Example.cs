using System;

struct Translation
{
	public float x;
	public float y;
	public float z;

	public override string ToString()
	{
		return "(x=" + x + "|y=" + y + "|z=" + z + ")";
	}
}

class TestScript
{
	Translation translation;

	void Start()
	{
	}

	void Update(float dt)
	{
		translation.x += 1.0f * dt;
		translation.y += 1.0f * dt;
		translation.z += 1.0f * dt;
	}
}
