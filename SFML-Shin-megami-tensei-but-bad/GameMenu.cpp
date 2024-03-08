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
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
            for (auto& it : worldData.worldObjects) {
                for (auto& vec : it) {
                    vec.x += cosf(angle - (M_PI / 4)) * 5.0f;
                    vec.y += sinf(angle-(M_PI/4) ) * 5.0f;
                }
            }

        }
        GameState::gameStateInstance->keys[UP] = sf::Keyboard::isKeyPressed(sf::Keyboard::W);
        GameState::gameStateInstance->keys[ROT_LEFT] = sf::Keyboard::isKeyPressed(sf::Keyboard::Q);
        GameState::gameStateInstance->keys[ROT_RIGHT] = sf::Keyboard::isKeyPressed(sf::Keyboard::E);
        

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
    sf::CircleShape p(20);
    auto&  playerData = *(RoamingState*)PlayerStateRegistrar::getInstance<RoamingState>().get();
    playerPos = playerData.playerPos; 
    p.setPosition(playerPos);
    p.setFillColor(sf::Color::Black);
    window->draw(p);
    for (auto it : worldData.worldObjects) {
        sf::VertexArray objToDraw(sf::LinesStrip, it.size());
        //std::cout << "its size is " << it.size() << std::endl; 
        // Assuming playerPosition is the center of rotation
        sf::Transform transform;
        transform.rotate((angle)*(180.0/M_PI), playerPos.x, playerPos.y);
        std::vector<sf::Vector2f> transformedPoints;
        for (int indexOfObjectVec = 0; indexOfObjectVec < it.size(); indexOfObjectVec++) {
            // Apply the rotation transform to each vertex
            sf::Vector2f rotatedPoint = transform.transformPoint(it[indexOfObjectVec]);
            transformedPoints.push_back(rotatedPoint);
            objToDraw[indexOfObjectVec].position.x = rotatedPoint.x ;
            objToDraw[indexOfObjectVec].position.y = rotatedPoint.y ;
            objToDraw[indexOfObjectVec].color = sf::Color::Black;
        }
        
        window->draw(objToDraw);


        //for (int i = 0; i <= 90; ++i) { // assuming angles are in degrees
        for (auto i = 1; i <= 4; ++i) {
            sf::VertexArray playerAngle(sf::LinesStrip, 2);
            playerAngle[0].color = sf::Color::Red;
            playerAngle[0].position = playerPos;
            playerAngle[1].color = sf::Color::Red;
            playerAngle[1].position.x = (cosf(angle - (M_PI / 4)) * 1000);
            playerAngle[1].position.y = (sinf(angle - (M_PI / 4)) * 1000);
            window->draw(playerAngle);
        }

        /*}*/
              

    }
    window->display();

}

