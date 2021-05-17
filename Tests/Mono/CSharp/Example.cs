using System;

class TestScript
{
	int classInteger;

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
	}
}
