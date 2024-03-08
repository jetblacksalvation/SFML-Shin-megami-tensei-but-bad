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
            PlayerStateRegistrar::PrintInfo();
            PlayerStateRegistrar::HandleChangeState<RoamingState>();
            PlayerStateRegistrar::PrintInfo();
		}
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
            for (auto& it : worldData.worldObjects) {
                for (auto& vec : it) {
                    vec.x += cosf(angle - M_PI + (M_PI / 4)) * 5.0f;
                    vec.y += sinf(angle - M_PI + (M_PI / 4)) * 5.0f;
                }
            }

        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q)) {
            angle -= 0.1;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::E)) {
            angle += 0.1;

        }
	}
    window->clear(sf::Color::White);
	draw2DScene(); 
}

void MenuState::OnLoad() {


}
void MenuState::draw2DScene() {
	auto worldData = Game::gameInstance->worldData;
    auto& window = Game::gameInstance->window; 
    auto& angle = GameState::gameStateInstance->angle;
    sf::CircleShape p(20);
    p.setPosition(playerPos);
    p.setFillColor(sf::Color::Black);
    window->draw(p);
    for (auto it : worldData.worldObjects) {
        sf::VertexArray objToDraw(sf::LinesStrip, it.size());
        //std::cout << "its size is " << it.size() << std::endl; 
        // Assuming playerPosition is the center of rotation
        sf::Transform transform;
        transform.rotate(angle, playerPos.x, playerPos.y);
        std::vector<sf::Vector2f> transformedPoints;
        for (int indexOfObjectVec = 0; indexOfObjectVec < it.size(); indexOfObjectVec++) {
            // Apply the rotation transform to each vertex
            sf::Vector2f rotatedPoint = transform.transformPoint(it[indexOfObjectVec]);
            transformedPoints.push_back(rotatedPoint);
            objToDraw[indexOfObjectVec].position.x = rotatedPoint.x;
            objToDraw[indexOfObjectVec].position.y = rotatedPoint.y;
            objToDraw[indexOfObjectVec].color = sf::Color::Black;
        }
        window->draw(objToDraw);

        //float windowWidth = static_cast<float>(window->getSize().x);
        //sf::Vector2f center(window->getSize().x / 2.0f, window->getSize().y / 2.0f);

        for (int i = 0; i <= 90; ++i) { // assuming angles are in degrees
            float angleInRadians = i * (M_PI / 180.0); // convert degrees to radians
            sf::VertexArray playerAngle(sf::LinesStrip, 2);
            playerAngle[0].color = sf::Color::Red;
            playerAngle[0].position = playerPos;
            playerAngle[1].color = sf::Color::Red;
            playerAngle[1].position.x = (cosf(angle+(M_PI/4)) * 1000) + playerPos.x;
            playerAngle[1].position.y = (sinf(angle + (M_PI / 4)) * 1000) + playerPos.y;
            window->draw(playerAngle);
        }
              

    }
    window->display();

}

