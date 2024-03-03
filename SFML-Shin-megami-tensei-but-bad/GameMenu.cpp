#include "GameState.h"
#include "GameMenu.h"
//MenuState();
//
//void HandleState();
//void OnLoad();
//void draw2DScene();

//
//
MenuState::MenuState() {

}
void MenuState::HandleState() {
	while (Game::gameInstance->window->pollEvent(GameState::event)) {
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape)) {
			PlayerStateRegistrar::HandleChangeState<RoamingState>();
		}

	}
	draw2DScene(); 
}

void MenuState::OnLoad() {


}
void MenuState::draw2DScene() {


}