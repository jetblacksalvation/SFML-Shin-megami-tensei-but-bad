
#pragma once
#include "Common.hpp"


#ifndef GameStates

#define GameStates

#include <SFML/Graphics.hpp>
#include <memory>

//apparently the fastest hash map in c++ stl
//eventually add scenario interpreter
class IPlayerState;

class GameState : public std::enable_shared_from_this<GameState>
{
public: 
    typedef std::shared_ptr<GameState>      TGameStateInstance; 
    typedef std::shared_ptr<IPlayerState>  TPlayerState;
    static TGameStateInstance               gameStateInstance;
    static TPlayerState                     playerStateInstance; 
    
	enum class PlayerStateEnums
	{
		Roaming = 0// roaming has nested 
	};
	PlayerStateEnums CurrentState;

    GameState();
};

class IPlayerState {
public:
	//variables 
	
	virtual void HandleState() {
		static_assert("ERR: This object has no defined virtual override for HanldeState!\n");
	}

};
class RoamingState;



class RoamingState : public IPlayerState {
public:
    sf::Vector2f playerPos = { 400,400 };

	void HandleState();
    
};

#endif
