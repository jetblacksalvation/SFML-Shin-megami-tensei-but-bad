#ifndef CommonMath

#include <cmath> 
#include <SFML/System.hpp>
#include <vector>
#define M_PI           3.14159265358979323846  /* pi */
#define CommonMath


//handles applying rotations to objects, and formating data.
class WorldHelper {
public:
    //TYPES
    typedef std::vector<sf::Vector2f> T_PrimitiveShape;
    //IE : a square or something like that .
    typedef std::vector<T_PrimitiveShape> T_WorldObjects;
    //IE : everything in the level, T_PrimitiveShape gets copied on world load
    //into instance of T_WorldObjects, this is so that the root data isn't changed, which can mess things up depending on design.
    //members
    T_WorldObjects worldObjects;

    //funcitons
    WorldHelper();
};
sf::Vector2f* calc_hit(sf::Vector2f p1, sf::Vector2f p2, sf::Vector2f p3, sf::Vector2f p4);

#endif // M_PI
