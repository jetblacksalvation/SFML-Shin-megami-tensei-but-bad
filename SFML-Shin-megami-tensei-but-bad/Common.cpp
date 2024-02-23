#pragma once

#include "Common.hpp"
Game::TWindowPtr Game::window;
void Game::HandleKeys(WorldHelper::T_WorldObjects& world_Data) {
    float temp = 0;//change to the orientation of angle 




}


Game::Game() {
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
            while (window->pollEvent(event)) {
                if (event.type == event.Closed) {
                    window->close();
                    exit(0);
                }



                if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
                    for (auto& it : worldData.worldObjects) {
                        for (auto& vec : it) {
                            //increase 
                            vec.x += cosf(angle-M_PI+(M_PI/4)) * 5.0f;
                            vec.y += sinf(angle-M_PI + (M_PI / 4)) * 5.0f;
                        }
                    }

                }
                keys[ROT_LEFT] = sf::Keyboard::isKeyPressed(sf::Keyboard::Q);

                keys[ROT_RIGHT] = sf::Keyboard::isKeyPressed(sf::Keyboard::E);


            }

            //eventually the movement will be changed, for now this will do.
            
            if (keys[ROT_LEFT]) {
                angle -= 0.05;
            }
            if (keys[ROT_RIGHT]) {
                angle += 0.05;
            }
            
            window->clear(sf::Color::White);
            draw3DScene(this->worldData.worldObjects, angle, playerPos);

            window->display();

        }
    }
void Game::draw3DScene(WorldHelper::T_WorldObjects& allWorldShapes, float angle, sf::Vector2f playerPos) {
    /*
        Eventually add in tiling data.
        Map should be 2d array, so yk world shapes should reflect that eventually

    */

    sf::Vector2u windowSize = window->getSize();

    int rayNum = 90; 

    for (auto it : allWorldShapes) {
        sf::VertexArray objToDraw(sf::LinesStrip, it.size());

        // Assuming playerPosition is the center of rotation
        sf::Transform transform;
        transform.rotate(angle, playerPos.x, playerPos.y);
        std::vector<sf::Vector2f> transformedPoints;
        for (int indexOfObjectVec = 0; indexOfObjectVec < it.size(); indexOfObjectVec++) {
            // Apply the rotation transform to each vertex
            sf::Vector2f rotatedPoint = transform.transformPoint(it[indexOfObjectVec]);
            transformedPoints.push_back(rotatedPoint);
            //objToDraw[indexOfObjectVec].position.x = rotatedPoint.x;
            //objToDraw[indexOfObjectVec].position.y = rotatedPoint.y;
            //objToDraw[indexOfObjectVec].color = sf::Color::Black;
        }
        window->draw(objToDraw);


        for (int i = 0; i <= rayNum; ++i) { // assuming angles are in degrees
            float angleInRadians = i * (M_PI / 180.0); // convert degrees to radians


            float distance = 0;
            sf::Vector2f interceptToDraw;
            sf::Vector2f p3Closest;
            sf::Vector2f p4Closest; 
            for (int x = 0; x < transformedPoints.size() - 1; x++) {
                playerPos;
                sf::Vector2f playerRayEnd;
                playerRayEnd.x = (cosf(angle + angleInRadians) * 1000) + playerPos.x;
                playerRayEnd.y = (sinf(angle + angleInRadians) * 1000) + playerPos.y;
                //sf::VertexArray objToDraw(sf::LinesStrip, 2);
                //objToDraw[0] = playerPos;
                //objToDraw[0].color = sf::Color::Black;


                //objToDraw[1] = playerRayEnd;
                //objToDraw[1].color = sf::Color::Black;

                //std::cout << "x :" << playerRayEnd.x << ", Y : " << playerRayEnd.y << "\n";
                auto p3 = it[x];
                auto p4 = it[x + 1];
                window->draw(objToDraw);
                sf::Vector2f* interceptPoint = calc_hit(p3, p4, playerPos, playerRayEnd);

                if (interceptPoint) {
                    if (distance > std::hypotf(playerPos.x - interceptPoint->x, playerPos.y - interceptPoint->y) or distance == 0) {
                        distance = std::hypotf(playerPos.x - interceptPoint->x, playerPos.y - interceptPoint->y);
                        interceptToDraw = *interceptPoint;
                        p3Closest = p3;
                        p4Closest = p4;
                        delete interceptPoint;
                        continue;
                    }
                    //ignore intercept if its distance is no good.
                    delete interceptPoint;

                }
            }

            if (distance > 0) {
                // Adjust the shading calculation. The further the distance, the darker the shade should be.
                sf::Vector2f wallVector = p4Closest - p3Closest;
                float correctedDistance = distance * cos(angle);

                float wallLength = std::sqrt(wallVector.x * wallVector.x + wallVector.y * wallVector.y);
                float distanceAlongWall = ((interceptToDraw.x - p3Closest.x) * (wallVector.x / wallLength)
                    + (interceptToDraw.y - p3Closest.y) * (wallVector.y / wallLength));
                // Normalize the distance to get texture coordinates in the range [0, 1]
                float textureX = distanceAlongWall / wallLength;


                float columnHeight = windowSize.y / (distance * 0.02f);  // Adjust the 0.02f as needed

                std::cout << "Column Hieght " << columnHeight << " \n";

                // Create the column with the calculated height
                sf::RectangleShape column(sf::Vector2f(windowSize.x / rayNum, columnHeight));


                int texX = static_cast<int>(textureX * texture.getSize().x);

                // Set the texture to the column
                column.setTexture(&texture);
                sf::IntRect textureRect(texX, 0, 1, texture.getSize().y);
                column.setTextureRect(textureRect);

                // Position the column
                column.setPosition(i * (windowSize.x / rayNum), (windowSize.y - columnHeight) / 2.0f);

                // Draw the column with the applied texture.
                window->draw(column);


            }

        }

    }

}


