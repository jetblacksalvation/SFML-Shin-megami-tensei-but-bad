#include "GameState.h"
GameState::TGameStateInstance GameState::gameStateInstance;
GameState::TPlayerState GameState::playerStateInstance;

GameState::GameState() {
	GameState::gameStateInstance = std::shared_ptr<GameState>(this);
	//default state to roaming, this will be changed later 
	GameState::playerStateInstance = std::make_shared<RoamingState>();

	auto game = *Game::gameInstance;


}
void draw3DScene(sf::Vector2f& playerPos);
void RoamingState::HandleState() {
	//std::cout << "IN ROAMING\n";
    draw3DScene(playerPos);
}

void draw3DScene( sf::Vector2f& playerPos) {
    Game& game = *Game::gameInstance; 
    WorldHelper::T_WorldObjects& allWorldShapes = game.worldData.worldObjects;
    Game::TWindowPtr& window = game.window;
        /*
            Eventually add in tiling data.
            Map should be 2d array, so yk world shapes should reflect that eventually

        */

        sf::Vector2u windowSize = window->getSize();

    int rayNum = 90;
    auto angle = game.angle;
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


                // Create the column with the calculated height
                sf::RectangleShape column(sf::Vector2f(windowSize.x / rayNum, columnHeight));


                int texX = static_cast<int>(textureX * Game::gameInstance->texture.getSize().x);

                // Set the texture to the column
                column.setTexture(&Game::gameInstance->texture);
                sf::IntRect textureRect(texX, 0, 1, Game::gameInstance->texture.getSize().y);
                column.setTextureRect(textureRect);

                // Position the column
                column.setPosition(i * (windowSize.x / rayNum), (windowSize.y - columnHeight) / 2.0f);

                // Draw the column with the applied texture.
                window->draw(column);


            }

        }

    }

}