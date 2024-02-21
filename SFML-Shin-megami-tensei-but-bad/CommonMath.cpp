#include "CommonMath.hpp"


WorldHelper::WorldHelper() {

}
sf::Vector2f* calc_hit(sf::Vector2f p1, sf::Vector2f p2, sf::Vector2f p3, sf::Vector2f p4) {
    // Calculates denominator of equations
    const float den = (p1.x - p2.x) * (p3.y - p4.y) - (p1.y - p2.y) * (p3.x - p4.x);

    if (den == 0)
        return nullptr;

    const float t = ((p1.x - p3.x) * (p3.y - p4.y) - (p1.y - p3.y) * (p3.x - p4.x)) / den;
    const float u = -((p1.x - p2.x) * (p1.y - p3.y) - (p1.y - p2.y) * (p1.x - p3.x)) / den;

    // If there's an intersection...
    sf::Vector2f* m_end = new sf::Vector2f();
    if (t >= 0 && t <= 1 && u >= 0 && u <= 1) {
        // Gets intersection point
        m_end->x = p1.x + t * (p2.x - p1.x);
        m_end->y = p1.y + t * (p2.y - p1.y);
    }
    else {
        delete m_end; // Clean up memory if there is no intersection
        m_end = nullptr;
    }

    return m_end;
}
