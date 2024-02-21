#pragma once
#include <SFML/Graphics.hpp>
#include "CommonMath.hpp"
#include <iostream>
/*
	COMMON TYPES, FUNCTIONS, ETC...

*/
// GLOBAL Variables 
enum DIRECTIONS { LEFT, RIGHT, UP, DOWN, ROT_LEFT, ROT_RIGHT }; //left is 0, right is 1, up is 2, down is 3

class Game {
public:
	float angle = 0;

	bool keys[6];
	//typedefs and helper types
	typedef std::unique_ptr<sf::RenderWindow> TWindowPtr;

	//member variables
	static  std::unique_ptr<sf::RenderWindow> window;
	WorldHelper worldData; 

	//functions
	Game();
	void run();
	void HandleKeys(std::vector<std::vector<sf::Vector2f>>& world_Data);
	
};



