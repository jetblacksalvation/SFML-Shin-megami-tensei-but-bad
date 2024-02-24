#pragma once

#include "Common.hpp"
Game::TWindowPtr Game::window;
Game::TGameInstance Game::gameInstance; // helper obj, its so that game state can grab whatever is needed.



Game::Game() {
    //instanceList.push_back(std::make_shared<Game>(this));
    gameInstance = std::shared_ptr<Game>(this);
    
    
    for (auto& k : keys) {
        k = false;
    }
    if (!window) {
        window = std::make_unique< sf::RenderWindow>();
        (*window).create({ 800,800 }, "Shin Megami Tensei");
        // Check if window creation was successful
        if (!window) {
            
            // Handle the error, perhaps throw an exception or exit the program
        }
    }
    else {
        // Handle the case where the window is already created
    }
    GameState::GameState();
}
void Game::run() {
    WorldHelper::T_PrimitiveShape points({ sf::Vector2f{100,100}, sf::Vector2f{200, 100}, sf::Vector2f{200,200 }, sf::Vector2f{100,200} , sf::Vector2f{100,100} });//last index is so that lines can wrap back to start 
    (*window).setFramerateLimit(120);
    sf::Event event;
    //square 
    sf::Vector2f playerPos = { 400,400 };

    texture.loadFromFile("brickWall.png"); 
    sprite.setTexture(texture);

    sf::Texture playerImage;

    if (!playerImage.loadFromFile("Arrow.png")) exit(0); //this line loads the image AND kills your program if it doesn't load
    sf::Sprite playerSprite;
    playerSprite.setTexture(playerImage);
    playerSprite.setPosition(playerPos.x - 16, playerPos.y - 16);//offset by half of pixel dimensions   

    worldData.worldObjects = { points };
    int camera_plane = playerPos.y / 2;
    std::cout << "Hello Rotation!\n";
    auto windowSize = window->getSize();

    while (window->isOpen()) {


        //eventually the movement will be changed, for now this will do.
            
        if (keys[ROT_LEFT]) {
            angle -= 0.05;
        }
        if (keys[ROT_RIGHT]) {
            angle += 0.05;
        }
            
        window->clear(sf::Color::White);
        //draw3DScene(this->worldData.worldObjects, angle, playerPos);
        GameState::playerStateInstance->HandleState();

        window->display();

    }
    exit(0);
}
