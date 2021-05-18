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
	int classInteger;
	Translation translation;

	TestScript() { Console.WriteLine("Constructor called"); }

	static void StaticMethod(int i)
	{
		Console.WriteLine("Static method called " + i.ToString());
	}

	void Start()
	{
		Console.WriteLine("Start::ClassInteger: " + classInteger.ToString());
	}

	void Update(int dt)
	{
		Console.WriteLine("Update::ClassInteger: " + classInteger.ToString());
		Console.WriteLine("Update::DeltaTime: " + dt.ToString());
		Console.WriteLine("Update::Translation: " + translation.ToString());
	}
}
