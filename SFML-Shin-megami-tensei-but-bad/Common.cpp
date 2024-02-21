#pragma once

#include "Common.hpp"
Game::TWindowPtr Game::window;
void Game::HandleKeys(WorldHelper::T_WorldObjects& world_Data) {
    float temp = 0;//change to the orientation of angle 




}


Game::Game() {
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
}
void Game::run() {
    WorldHelper::T_PrimitiveShape points({ sf::Vector2f{100,100}, sf::Vector2f{200, 100}, sf::Vector2f{200,200 }, sf::Vector2f{100,200} , sf::Vector2f{100,100} });//last index is so that lines can wrap back to start 
    (*window).setFramerateLimit(120);
    sf::Event event;
    //square 
    sf::Vector2f playerPos = { 400,400 };




    sf::Texture playerImage;

    if (!playerImage.loadFromFile("Arrow.png")) exit(0); //this line loads the image AND kills your program if it doesn't load
    sf::Sprite playerSprite;
    playerSprite.setTexture(playerImage);
    playerSprite.setPosition(playerPos.x - 16, playerPos.y - 16);//offset by half of pixel dimensions   

    worldData.worldObjects = { points };
    int camera_plane = playerPos.y / 2;
    std::cout << "Hello Rotation!\n";

    while (window->isOpen()) {
            while (window->pollEvent(event)) {
                if (event.type == event.Closed) {
                    window->close();
                    exit(0);
                }



                if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
                    for (auto& it : worldData.worldObjects) {
                        for (auto& vec : it) {
                            vec.x += cosf(angle-M_PI+(M_PI/6)) * 5.0f;
                            vec.y += sinf(angle-M_PI + (M_PI / 6)) * 5.0f;
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

            {        //input handleing here 

                if (keys[ROT_LEFT]) {
                    angle -= 0.05;
                }
                if (keys[ROT_RIGHT]) {
                    angle += 0.05;
                }
            }
            window->clear(sf::Color::White)
                ;            //for (auto it : this->worldData.worldObjects) {//<--- working
            //    sf::VertexArray objToDraw(sf::LinesStrip, it.size());

            //    // Assuming playerPosition is the center of rotation
            //    sf::Transform transform;
            //    transform.rotate(angle, playerPos.x, playerPos.y);
            //    for (int indexOfObjectVec = 0; indexOfObjectVec < it.size(); indexOfObjectVec++) {
            //        // Apply the rotation transform to each vertex
            //        sf::Vector2f rotatedPoint = transform.transformPoint(it[indexOfObjectVec]);
            //        objToDraw[indexOfObjectVec].position.x = rotatedPoint.x;
            //        objToDraw[indexOfObjectVec].position.y = rotatedPoint.y;
            //        objToDraw[indexOfObjectVec].color = sf::Color::Black;
            //    }

            //    window->draw(objToDraw);
            //}
            //test
            for (auto it : this->worldData.worldObjects) {
                sf::VertexArray objToDraw(sf::LinesStrip, it.size());

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

                float windowWidth = static_cast<float>(window->getSize().x);
                sf::Vector2f center(window->getSize().x / 2.0f, window->getSize().y / 2.0f);

                for (int i = 0; i <= 90; ++i) { // assuming angles are in degrees
                    float angleInRadians = i * (3.14159 / 180.0); // convert degrees to radians

                    for (int x = 0; x < transformedPoints.size() - 1; x++) {
                        playerPos;
                        sf::Vector2f playerRayEnd;
                        sf::VertexArray objToDraw(sf::LinesStrip, 2);
                        objToDraw[0] = playerPos;
                        objToDraw[0].color = sf::Color::Black;

                        playerRayEnd.x = (cosf(angle+ angleInRadians) * 1000) + playerPos.x;
                        playerRayEnd.y = (sinf(angle+ angleInRadians) * 1000) + playerPos.y;
                        objToDraw[1] = playerRayEnd;
                        objToDraw[1].color = sf::Color::Black;

                        //std::cout << "x :" << playerRayEnd.x << ", Y : " << playerRayEnd.y << "\n";
                        auto p3 = it[x];
                        auto p4 = it[x + 1];
                        window->draw(objToDraw);
                        sf::Vector2f* interceptPoint = calc_hit(playerPos, playerRayEnd, p3, p4);

                        if (interceptPoint) {
                            float distance = std::hypotf(playerPos.x - interceptPoint->x, playerPos.y - interceptPoint->y);
                            float columnWidth = 10.0f;  // Width of the column to be drawn
                            std::cout << i << " index\n";
                            // Draw a column based on the distance
                            sf::RectangleShape column(sf::Vector2f(20, distance));
                            column.setPosition((center.x)/i*25, distance/2);
                            column.setFillColor(sf::Color::Red);  // Set the color as needed
                            window->draw(column);

                            delete interceptPoint;
                        }
                    }
                    sf::VertexArray playerAngle(sf::LinesStrip, 2);
                    playerAngle[0].color = sf::Color::Red;
                    playerAngle[0].position = playerPos;
                    playerAngle[1].color = sf::Color::Red;
                    playerAngle[1].position.x = (cosf(angle) * 1000) + playerPos.x;
                    playerAngle[1].position.y = (sinf(angle) * 1000) + playerPos.y;
                    window->draw(playerAngle);
                }
              

            }
            //playerSprite.setPosition(playerPos.x - 16, playerPos.y - 16);//offset by half of pixel dimensions   
            //playerSprite.setRotation(angle);

            //window->draw(playerSprite);

            window->display();

        }
    }


