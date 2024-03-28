#include "GameState.h"
RoamingState::GridHelper::GridHelper(RoamingState* state) {
    this->gridData = { { 1,1,1,1,1,1,1 },
    { 1,0,0,0,0,0,1 },
    { 1,1,1,1,1,1,1 } };
    const int scaleFactor = 100;
    WorldHelper::T_WorldObjects& allWorldShapes = Game::gameInstance->worldData.worldObjects;
    std::cout << "Populating world...\n";
    auto& playerData = *state;

    //my stupid ahh, if x=0 the walls would be screwed up. so i have magic number here to offset when needed, and it starts at 1 so calcs dont get fucked 
    WorldHelper::T_PrimitiveShape points({ sf::Vector2f{-50,50}, sf::Vector2f{50, 50}, sf::Vector2f{-50,50 }, sf::Vector2f{-50,150},  sf::Vector2f{50,150},sf::Vector2f{50,50 } });//last index is so that lines can wrap back to start 

    int vertexCount = 0;
    for (uint32_t x = 1; x <= this->gridData.size(); x++) {
        for (uint32_t y = 1; y <= this->gridData[x-1].size(); y++) {
            WorldHelper::T_PrimitiveShape temp;
            if (gridData[x - 1][y - 1] == 1) {
                //((x - LoaderInstance.PlayerPosition[0]) * 2, (y - LoaderInstance.PlayerPosition[1]) * 2)
                for (auto point : points) {
                    try {
                        temp.push_back({ point.x + ((x)) * scaleFactor, point.y + ((y)*scaleFactor) });

                    }
                    catch (...) {

                    }

                }

                //std::cout << x << ": " << point.x + ((x) *scaleFactor) << ", " << point.y + ((y) *scaleFactor) <<" OBJECT" << std::endl;
            }

            if (gridData[x - 1][y - 1] == 2) {
                std::cout << x << ", " << y << std::endl;
                playerData.setGridPos({ x,(y ) });
            }

            if (temp.size() > 0) {
                vertexCount++;

                //std::cout << vertexCount << " has a size of " << temp.size() << std::endl;
                allWorldShapes.push_back(temp);

            }
        }
    }
}
void RoamingState::GridHelper::gridToWorld() {

}
RoamingState::GridHelper::GridHelper(std::ifstream file) {
    std::string line; 
    std::vector<std::vector<int>> gData{};

    while (std::getline(file, line))
    {
        std::vector<int> line{};

        std::string intBuff; 
        for (auto it : line) {
            if (std::isdigit(it)) {
                intBuff += it;

            }
            else if(it == '{') {
                if (intBuff.size() > 0) {
                    throw std::runtime_error("parse err");
                }
            }
            else if (it == '}') {
                
                line.push_back(std::stoi(intBuff));
                gData.push_back(line);
                continue; 
            }
            if (it == ',') {
                line.push_back(std::stoi(intBuff));
                gData.push_back(line);
            }
            
        }
        
        //if (line != line.end()) {

        //}

       


        // process pair (a,b)
    }
	//file.is_open
}