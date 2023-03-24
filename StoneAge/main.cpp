#include <SFML/Window.hpp>
#include <iostream>
#include "engine.h"
#include "array3d.h"

int main()
{
#pragma region TesTArray3D

	//Test Array3d
	/*Array3d<int> TestArray3D;

	TestArray3D = Array3d<int>(16,128,16);

	for (int i = 0; i < 16; i++)
	{
		for (int j = 0; j < 16; j++)
		{
			for (int k = 0; k < 128; k++)
			{
				TestArray3D.Set(i, j, k, 6);
			}
		}
	}

	for (int i = 0; i < 16; i++)
	{
		for (int j = 0; j < 16; j++)
		{
			for (int k = 0; k < 128; k++)
			{
				std::cout << TestArray3D.Get(i, j, k) << std::endl;
			}
		}
	}

	TestArray3D.Set(6, 5, 9, 8);
	std::cout << TestArray3D.Get(6, 5, 9) << std::endl;*/
#pragma endregion

	Engine engine;
	engine.SetMaxFps(60);
	engine.Start("StoneAge", 800, 600, false);
}