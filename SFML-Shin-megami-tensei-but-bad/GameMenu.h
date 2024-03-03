#pragma once
/*
	Handles the Menu and its substates.

*/


#include "GameState.h"
class MenuState : public IPlayerState {
public:
    
    MenuState();

    void HandleState();
    void OnLoad();
    void draw2DScene();

};

