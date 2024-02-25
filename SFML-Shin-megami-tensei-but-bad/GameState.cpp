#include "GameState.h"
//re-decs 

GameState::TGameStateInstance GameState::gameStateInstance;
GameState::TPlayerState       GameState::playerStateInstance;
bool                          GameState::keys[];
float                         GameState::angle;

sf::Event                     GameState::event;
//funcs 
GameState::GameState() {
	GameState::gameStateInstance = std::shared_ptr<GameState>(this);
	//default state to roaming, this will be changed later 
	GameState::playerStateInstance = std::make_shared<RoamingState>();

    for (auto& k : keys) {
        k = false;
    }
    angle = M_PI/4;

}
RoamingState::RoamingState() {
    texture.loadFromFile("brickWall.png");
    sprite.setTexture(texture);
}
void RoamingState::HandleState() {
    int worldD[10][20];
    std::cout << "The P_angle = " << GameState::gameStateInstance->angle << std::endl; 
    std::cout << RoamingState::playerPos.x << ", " << RoamingState::playerPos.y << "\n";
    while (Game::gameInstance->window->pollEvent(GameState::event)) {      
        if (GameState::event.type == GameState::event.Closed) {
            std::cout << "Closing\n";
            Game::gameInstance->window->close();
            abort();
        }


        GameState::gameStateInstance->keys[UP]        = sf::Keyboard::isKeyPressed(sf::Keyboard::W);
        GameState::gameStateInstance->keys[ROT_LEFT]  = sf::Keyboard::isKeyPressed(sf::Keyboard::Q);
        GameState::gameStateInstance->keys[ROT_RIGHT] = sf::Keyboard::isKeyPressed(sf::Keyboard::E);


    }
    if (GameState::gameStateInstance->keys[UP]) {
        for (auto& it : Game::gameInstance->worldData.worldObjects) {
            for (auto& vec : it) {
                //increase 
                vec.x += cosf(GameState::gameStateInstance->angle - M_PI + (M_PI / 4)) * 5.0f;
                vec.y += sinf(GameState::gameStateInstance->angle - M_PI + (M_PI / 4)) * 5.0f;
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
	//std::cout << "IN ROAMING\n";
    draw3DScene();
}

void RoamingState::draw3DScene() {
    Game& game = *Game::gameInstance; 
    WorldHelper::T_WorldObjects& allWorldShapes = game.worldData.worldObjects;
    Game::TWindowPtr& window = game.window;
    /*
        Eventually add in tiling data.
        Map should be 2d array, so yk world shapes should reflect that eventually

    */

    sf::Vector2u windowSize = window->getSize();

    int rayNum = 90;
    auto angle = GameState::gameStateInstance->angle;
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


                int texX = static_cast<int>(textureX * texture.getSize().x);

                // Set the texture to the column
                column.setTexture(&texture);
                sf::IntRect textureRect(texX, 0, 1,texture.getSize().y);
                column.setTextureRect(textureRect);

                // Position the column
                column.setPosition(i * (windowSize.x / rayNum), (windowSize.y - columnHeight) / 2.0f);

                // Draw the column with the applied texture.
                window->draw(column);


            }

        }

    }

}