#include "GameState.h"
//re-decs 

std::unordered_map<std::type_index, std::shared_ptr<IPlayerState>> PlayerStateRegistrar::instances;

GameState::TGameStateInstance GameState::gameStateInstance;
GameState::TPlayerState       GameState::playerStateInstance;
bool                          GameState::keys[];
float                         GameState::angle;

sf::Event                     GameState::event;
//funcs 
void RoamingState::setGridPos(const sf::Vector2<uint32_t>&& vec) {
    setGridPos(vec);
};
void RoamingState::setGridPos(const sf::Vector2<uint32_t>& vec) {
    this->gridPos = (sf::Vector2u) vec;
    playerPos = sf::Vector2f{ ((float)(vec.x)*100 )+100,((float)(vec.y )*100 )+200};

};

void RoamingState::OnLoad() {
    setGridPos({ 1,2 });
    //offset each point via player offset in world array

    windowSize = Game::gameInstance->window->getSize(); 

    //a scale factor of ten seems to work ?

   
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
RoamingState::RoamingState() {
    std::cout << "Roaming State Instantiated!\n";
    gridData = GridHelper(this);

    texture.loadFromFile("brickWall.png");
    sprite.setTexture(texture);
    OnLoad(); 
}
void RoamingState::MovePlayer() {
    std::cout << faceIndex << " is face index\n";

    sf::Vector2u newPosition = gridPos;

    switch (faceIndex) {
    case 0:
        newPosition.y += 1;
        break;
    case 1:
        newPosition.x -= 1;
        newPosition.y -= 1;
        break;
    case 2:
        newPosition.y -= 1;
        break;
    case 3:
        newPosition.x += 1;
        break;
    default:
        std::cerr << "Invalid face index\n";
        return;
    }

    if (newPosition.x < gridData.gridData.size() && newPosition.y < gridData.gridData[newPosition.x].size()) {
        if (gridData.gridData[newPosition.x][newPosition.y] == 0) {
            setGridPos(newPosition);
        }
    }
}


void RoamingState::HandleState() {

    //std::cout << allWorldShapes.size() << " is size\n";

    //std::cout << "The P_angle = " << GameState::gameStateInstance->angle << std::endl; 
    //std::cout << RoamingState::ppos.x << ", " << RoamingState::ppos.y << "\n";
    const std::clock_t c_start = std::clock();
    
    std::chrono::steady_clock::time_point t_start = std::chrono::high_resolution_clock::now();

    // do something

    while (Game::gameInstance->window->pollEvent(GameState::event) &&!this->blockInput) {      
        if (GameState::event.type == GameState::event.Closed) {
            std::cout << "Closing\n"; 
            Game::gameInstance->window->close();
            abort();
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::O)) {
            std::cout << "ECHO PPOS\n";
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

        
        GameState::gameStateInstance->keys[ROT_RIGHT] = sf::Keyboard::isKeyPressed(sf::Keyboard::E);
        GameState::gameStateInstance->keys[ROT_LEFT] = sf::Keyboard::isKeyPressed(sf::Keyboard::Q);


        GameState::gameStateInstance->keys[UP] = sf::Keyboard::isKeyPressed(sf::Keyboard::W);
        GameState::gameStateInstance->keys[LEFT] = sf::Keyboard::isKeyPressed(sf::Keyboard::A);
        GameState::gameStateInstance->keys[DOWN] = sf::Keyboard::isKeyPressed(sf::Keyboard::S);
        GameState::gameStateInstance->keys[RIGHT] = sf::Keyboard::isKeyPressed(sf::Keyboard::D);




    }
    //float offset = M_PI / 4;
    //int face = 0;
    //float faces[4] = { 0 + offset, (M_PI / 2) + offset, (M_PI)+offset, 3 * (M_PI / 2) + offset };
    if (GameState::gameStateInstance->keys[UP]) {
        std::cout << gridPos.x << ", " << gridPos.y << std::endl;

        std::cout << "max = " << gridData.gridData.size() << ", " << gridData.gridData[gridPos.x].size() << std::endl; 
        std::cout << "facesIndex :" << faces[faceIndex] << std::endl;

        //PlayerStateRegistrar::HandleChangeState<MenuState>();

        MovePlayer();


        std::this_thread::sleep_for(std::chrono::milliseconds(250));

        //this handles rotations, for the game world. this works by using the player as the center of the transformation ... 
    }
    //offset anlge by M_PI 

    if (GameState::gameStateInstance->keys[ROT_LEFT]) {
        (this->faceIndex - 1 < 0) ? faceIndex = 3 : this->faceIndex--;

    }
    if (GameState::gameStateInstance->keys[ROT_RIGHT]){
    
        (this->faceIndex + 1 > 3) ? faceIndex = 0 : faceIndex++;
        //delta of this 
    }
    std::cout << "INDEX = " << faceIndex << std::endl;

    if (GameState::gameStateInstance->keys[ROT_LEFT] || GameState::gameStateInstance->keys[ROT_RIGHT]) {
        float delta = (faces[faceIndex] - GameState::gameStateInstance->angle)/2;
        GameState::gameStateInstance->angle += delta; 
        draw3DScene();
        std::this_thread::sleep_for(std::chrono::milliseconds(250));
        //a little smother feeling way of turning the character... 

    }
    if (GameState::gameStateInstance->angle < 0.0) {
        GameState::gameStateInstance->angle += 2 * M_PI;
    }
    else if (GameState::gameStateInstance->angle >= 2 * M_PI) {
        GameState::gameStateInstance->angle -= 2 * M_PI;
    }
	//std::cout << "IN ROAMING\n";
    GameState::gameStateInstance->angle = faces[faceIndex];


    draw3DScene();
    
}

void RoamingState::draw3DScene() {
    //std::cout << "dwwr \n";
    Game& game = *Game::gameInstance.get(); 
    WorldHelper::T_WorldObjects& allWorldShapes = game.worldData.worldObjects;
    Game::TWindowPtr& window = game.window;
    //std::cout << playerPos.x << ", " << playerPos.y << std::endl;;

    int rayNum = 100;
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
