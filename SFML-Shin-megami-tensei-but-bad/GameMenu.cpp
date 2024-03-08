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
    std::cout << "Menu state created\n";
    windowSize = Game::gameInstance->window->getSize(); 
}
void MenuState::HandleState() {
    //std::cout << "menu state active\n";
    
    auto& angle = GameState::gameStateInstance->angle;
    auto& window = Game::gameInstance->window;
    auto& worldData = Game::gameInstance->worldData;

	while (Game::gameInstance->window->pollEvent(GameState::event)) {
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape)) {

            auto ptr = PlayerStateRegistrar::getInstance<RoamingState>();
            ptr->isLoaded = true; 
            //std::cout << ptr->isLoaded << " roaming\n";
            //PlayerStateRegistrar::PrintInfo();
            PlayerStateRegistrar::HandleChangeState<RoamingState>();
            //PlayerStateRegistrar::PrintInfo();
		}

        GameState::gameStateInstance->keys[UP] = sf::Keyboard::isKeyPressed(sf::Keyboard::W);
        GameState::gameStateInstance->keys[ROT_LEFT] = sf::Keyboard::isKeyPressed(sf::Keyboard::Q);
        GameState::gameStateInstance->keys[ROT_RIGHT] = sf::Keyboard::isKeyPressed(sf::Keyboard::E);
        

	}
    if (GameState::gameStateInstance->keys[UP]) {
        for (auto& it : worldData.worldObjects) {
            for (auto& vec : it) {
                vec.x += cosf(angle - (3 * (M_PI / 4))) * 2.5f;
                vec.y += sinf(angle - (3 * (M_PI / 4))) * 2.5f;
            }
        }

        
    }
    if (GameState::gameStateInstance->keys[ROT_LEFT]) {
        GameState::gameStateInstance->angle -= 0.05;
    }
    if (GameState::gameStateInstance->keys[ROT_RIGHT]) {
        GameState::gameStateInstance->angle += 0.05;
    }
    if (GameState::gameStateInstance->angle < 0.0) {
        GameState::gameStateInstance->angle += 2 * M_PI;
    }
    else if (GameState::gameStateInstance->angle >= 2 * M_PI) {
        GameState::gameStateInstance->angle -= 2 * M_PI;
    }
    window->clear(sf::Color::White);
	draw2DScene(); 
}

void MenuState::OnLoad() {


}
void MenuState::draw2DScene() {
	auto worldData = Game::gameInstance->worldData;
    auto& window = Game::gameInstance->window; 
    float angle = GameState::gameStateInstance->angle;
    auto&  playerData = *(RoamingState*)PlayerStateRegistrar::getInstance<RoamingState>().get();
    playerPos = playerData.playerPos; 
    //playerData.sprite.setPosition(playerPos);
    sf::Texture texture; 

    texture.loadFromFile("Arrow.png");
    sf::Sprite sprite; 
    sprite.setTexture(texture);
    sprite.setPosition({ 400,400 });
    sprite.setRotation((angle +(  3*(M_PI / 4))) * (180.0 / M_PI));
    sprite.setOrigin(16, 16);
    window->draw(sprite);
    for (auto it : worldData.worldObjects) {
        sf::VertexArray objToDraw(sf::LinesStrip, it.size());
        for (int indexOfObjectVec = 0; indexOfObjectVec < it.size(); indexOfObjectVec++) {
            // Apply the rotation transform to each vertex
            sf::Vector2f rotatedPoint = it[indexOfObjectVec];
            objToDraw[indexOfObjectVec].position.x = rotatedPoint.x -playerPos.x +400;
            objToDraw[indexOfObjectVec].position.y = rotatedPoint.y -playerPos.y+400;
            objToDraw[indexOfObjectVec].color = sf::Color::Black;
        }
        
        window->draw(objToDraw);


    }
    window->display();

}

