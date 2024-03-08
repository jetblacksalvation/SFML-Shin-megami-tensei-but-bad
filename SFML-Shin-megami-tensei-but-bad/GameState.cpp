#include "GameState.h"
//re-decs 

std::unordered_map<std::type_index, std::shared_ptr<IPlayerState>> PlayerStateRegistrar::instances;

GameState::TGameStateInstance GameState::gameStateInstance;
GameState::TPlayerState       GameState::playerStateInstance;
bool                          GameState::keys[];
float                         GameState::angle;

sf::Event                     GameState::event;
//funcs 


void RoamingState::OnLoad() {
    if (isLoaded == true) { return; }
    if (Game::gameInstance->worldData.worldObjects.size() > 3) {
        return;
    }
    std::cout << "is loaded!\n";
    WorldHelper::T_PrimitiveShape points({ sf::Vector2f{-50,50}, sf::Vector2f{50, 50}, sf::Vector2f{-50,50 }, sf::Vector2f{-50,150},  sf::Vector2f{50,150},sf::Vector2f{50,50 } });//last index is so that lines can wrap back to start 
    //offset each point via player offset in world array

    windowSize = Game::gameInstance->window->getSize(); 
    int worldData[3][8] = {
        {1,1,1,1,1,1,1},
        {1,2,0,0,0,0,1},
        {1,1,1,1,1,1,1}

    };
    //a scale factor of ten seems to work ?

    const int scaleFactor = 100; 
    WorldHelper::T_WorldObjects& allWorldShapes = Game::gameInstance->worldData.worldObjects;
    std::cout << "Populating world...\n";
    //my stupid ahh, if x=0 the walls would be screwed up. so i have magic number here to offset when needed, and it starts at 1 so calcs dont get fucked 

    int vertexCount = 0;
    for (int x = 1; x <= 3; x++) {
        for (int y = 1; y <= 8; y++) {
            WorldHelper::T_PrimitiveShape temp;

            for (auto point : points) {

                if (worldData[x-1][y-1] == 1) {
                    //((x - LoaderInstance.PlayerPosition[0]) * 2, (y - LoaderInstance.PlayerPosition[1]) * 2)
                    if (x == 0) {
                        point.x *= scaleFactor; 
                    }
                    if (y == 0) {
                        point.y *= scaleFactor;
                    }
                    temp.push_back({ point.x + ((x )) *scaleFactor, point.y + ((y ) *scaleFactor) });

                    //std::cout << x << ": " << point.x + ((x) *scaleFactor) << ", " << point.y + ((y) *scaleFactor) <<" OBJECT" << std::endl;
                }
                if (worldData[x-1][y-1] == 2) {
                    std::cout << x << ", " << y << std::endl; 
                    playerPos = sf::Vector2f{ (float)(x) * scaleFactor,(float) (y+1) * scaleFactor };
                }

            }
            if (temp.size() > 0) {
                vertexCount++;

                //std::cout << vertexCount << " has a size of " << temp.size() << std::endl;
                allWorldShapes.push_back(temp);

            }
        }
    }
    isLoaded = true; 
    std::cout << allWorldShapes.size() << ", is the size at end of funct\n";
}

GameState::GameState() {
	GameState::gameStateInstance = std::shared_ptr<GameState>(this);
    
	//default state to roaming, this will be changed later 
    
    for (auto& k : keys) {
        k = false;
    }
    angle = M_PI/4;
    //PlayerStateRegistrar::registerInstance<RoamingState>(); 
    PlayerStateRegistrar::HandleChangeState<RoamingState>();
    //GameState::playerStateInstance = PlayerStateRegistrar::getInstance<RoamingState>();
}
RoamingState::RoamingState()  {
    std::cout << "Roaming State Instantiated!\n";

    texture.loadFromFile("brickWall.png");
    sprite.setTexture(texture);
    OnLoad(); 
}
void RoamingState::HandleState() {
   

    //std::cout << allWorldShapes.size() << " is size\n";

    //std::cout << "The P_angle = " << GameState::gameStateInstance->angle << std::endl; 
    //std::cout << RoamingState::ppos.x << ", " << RoamingState::ppos.y << "\n";
    while (Game::gameInstance->window->pollEvent(GameState::event)) {      
        if (GameState::event.type == GameState::event.Closed) {
            std::cout << "Closing\n"; 
            Game::gameInstance->window->close();
            abort();
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::O)) {
            std::cout << "Set PPos\n";
            std::cout << "current pos = " << playerPos.x << ", " << playerPos.y << std::endl; 
            std::cin >> playerPos.x;
            std::cin >> playerPos.y;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::M)) {
            //show 2d representation for the world...
            //it is seperate state, for menu. for now its debug 
            PlayerStateRegistrar::HandleChangeState<MenuState>();
            return;
        }

        GameState::gameStateInstance->keys[UP]        = sf::Keyboard::isKeyPressed(sf::Keyboard::W);
        GameState::gameStateInstance->keys[ROT_LEFT]  = sf::Keyboard::isKeyPressed(sf::Keyboard::Q);
        GameState::gameStateInstance->keys[ROT_RIGHT] = sf::Keyboard::isKeyPressed(sf::Keyboard::E);


    }
    if (GameState::gameStateInstance->keys[UP]) {

        //this handles rotations, for the game world. this works by using the player as the center of the transformation ... 

        //for (auto& it : Game::gameInstance->worldData.worldObjects) {
        //    for (auto& vec : it) {
        //        //increase 
        //        vec.x += cosf(GameState::gameStateInstance->angle - M_PI + (M_PI / 4)) * 5.0f;
        //        vec.y += sinf(GameState::gameStateInstance->angle - M_PI + (M_PI / 4)) * 5.0f;
        //    }
        //}
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
    //std::cout << "dwwr \n";
    Game& game = *Game::gameInstance.get(); 
    WorldHelper::T_WorldObjects& allWorldShapes = game.worldData.worldObjects;
    Game::TWindowPtr& window = game.window;
    //std::cout << playerPos.x << ", " << playerPos.y << std::endl;;


    /*
        Eventually add in tiling data.
        Map should be 2d array, so yk world shapes should reflect that eventually

    */


    int rayNum = 90;
    auto angle = GameState::gameStateInstance->angle;

    for (int i = 0; i <= rayNum; ++i) { // assuming angles are in degrees
        float distance = 0;
        sf::Vector2f interceptToDraw;
        sf::Vector2f p3Closest;
        sf::Vector2f p4Closest;
        for (auto it : allWorldShapes) {
            //sf::VertexArray objToDraw(sf::LinesStrip, it.size());

            // Assuming playerPosition is the center of rotation
            sf::Transform transform;
            //std::cout << "PPos " << playerPos.x << ", " << playerPos.y << std::endl;
            transform.rotate(angle, playerPos.x, playerPos.y);
            std::vector<sf::Vector2f> transformedPoints;
            for (int indexOfObjectVec = 0; indexOfObjectVec < it.size(); indexOfObjectVec++) {
                // Apply the rotation transform to each vertex

                sf::Vector2f rotatedPoint = transform.transformPoint(it[indexOfObjectVec]);
                transformedPoints.push_back(rotatedPoint);

            }
            //window->draw(objToDraw);


            float angleInRadians = i * (M_PI / 180.0); // convert degrees to radians


            //sf::Vector2f interceptToDraw;
            //sf::Vector2f p3Closest;
            //sf::Vector2f p4Closest;
            for (int x = 0; x < it.size() - 1; x++) {
                playerPos;
                sf::Vector2f playerRayEnd;
                playerRayEnd.x = (cosf(angle + angleInRadians) * 1000) + playerPos.x;
                playerRayEnd.y = (sinf(angle + angleInRadians) * 1000) + playerPos.y;

                auto p3 = it[x];
                auto p4 = it[x + 1];
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
        }
        if (distance > 0) {
            // Adjust the shading calculation. The further the distance, the darker the shade should be.
            sf::Vector2f wallVector = p4Closest - p3Closest;
            float correctedDistance = distance * cos(angle);
            //std::cout << distance << " is d, " << std::endl;

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
            sf::IntRect textureRect(texX, 0, 1, texture.getSize().y);
            column.setTextureRect(textureRect);

            // Position the column
            column.setPosition(i * (windowSize.x / rayNum), (windowSize.y - columnHeight) / 2.0f);

            // Draw the column with the applied texture.
            window->draw(column);


        }

    }
    window->display(); 

}

//PlayerState::GameState()
//{
//}
