#pragma once
#ifndef GameCommon
#define GameCommon
#include <SFML/Graphics.hpp>
#include "CommonMath.hpp"
#include <iostream>
#include <unordered_map>
#include <memory>
#include "GameState.h"
/*
	COMMON TYPES, FUNCTIONS, ETC...

*/
// GLOBAL Variables 
enum DIRECTIONS { LEFT, RIGHT, UP, DOWN, ROT_LEFT, ROT_RIGHT }; //left is 0, right is 1, up is 2, down is 3

class Game:public std::enable_shared_from_this<Game>{
public:
	float angle = 0;
	sf::Texture texture;
	sf::Sprite sprite;

	bool keys[6];
	//typedefs and helper types
	typedef std::unique_ptr<sf::RenderWindow> TWindowPtr;
	typedef std::shared_ptr<Game> TGameInstance; 
	//member variables

	static  TWindowPtr window;
	WorldHelper worldData; 
	static TGameInstance gameInstance; // helper obj, its so that game state can grab whatever is needed.
	//functions
	Game();
	void run();

};



#endif