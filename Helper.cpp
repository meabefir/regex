#include "Helper.h"
#include <math.h>
#include "Mouse.h"

float Helper::VectorDistance(const sf::Vector2f& v1, const sf::Vector2f& v2)
{
    return sqrt(pow(v1.x - v2.x, 2) + pow(v1.y - v2.y, 2));
}

float Helper::VectorDistance(const sf::Vector2i& v1, const sf::Vector2f& v2)
{
    return sqrt(pow(v1.x - v2.x, 2) + pow(v1.y - v2.y, 2));
}

float Helper::VectorLength(const sf::Vector2f& v)
{
    return sqrt(pow(v.x, 2) + pow(v.y, 2));
}

sf::Vector2f Helper::GetNormalizedVector(const sf::Vector2f& v)
{
    float length = Helper::VectorLength(v);
    if (length != 0)
        return sf::Vector2f(v.x / length, v.y / length);
    return (sf::Vector2f(0.f, 0.f));
}

sf::Vector2f Helper::vectorRotatedRad(sf::Vector2f v, float rad)
{
    return sf::Vector2f(cos(rad) * v.x - sin(rad) * v.y,
                        sin(rad) * v.x + cos(rad) * v.y);
}

bool Helper::mouseInBox(sf::FloatRect rect)
{
    sf::Vector2f mp = Mouse::mousePosView;
    if (mp.x >= rect.left && mp.y >= rect.top && 
        mp.x <= rect.left + rect.width && mp.y <= rect.top + rect.height)
        return true;
    return false;
}
