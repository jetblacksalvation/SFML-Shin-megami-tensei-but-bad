#pragma once
/*
	Handles the Menu and its substates.

*/


#include "GameState.h"
class MenuState : public IPlayerState {
public:
    
    MenuState();
    sf::Vector2f playerPos = {400,400};
    sf::Vector2u windowSize;
    sf::Font font; 
    void HandleState();
    void OnLoad();
    void draw2DScene();
private:
};

