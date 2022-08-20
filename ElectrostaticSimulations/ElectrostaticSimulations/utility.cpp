#include "utility.h"

sf::Vector2f rotatePoint(const sf::Vector2f& point, const sf::Vector2f& center, const float& angle) {
	float a = point.x - center.x;
	float b = point.y - center.y;
	float sint = std::sinf(angle);
	float cost = std::cosf(angle);
	float x = a * cost + b * sint + center.x;
	float y = a * sint + b * cost + center.y;
	return { x, y };
}

float map(const float& value, const float& min, const float& max, const float& minRange, const float& maxRange) {
	return (minRange + (maxRange - minRange) * ((value - min) / (max - min)));
}

float dist2(const sf::Vector2f& a, const sf::Vector2f& b) {
	return ((a.x - b.x) * (a.x - b.x)) + ((a.y - b.y) * (a.y - b.y));
}

float dist2(const float& x, const float& y, const float& x2, const float& y2) {
	return (x - x2) * (x - x2) + (y - y2) * (y - y2);
}

float mag(const sf::Vector2f& a) {
	return std::sqrt((a.x * a.x) + (a.y * a.y));
}

float mag2(const sf::Vector2f& a) {
	return (a.x * a.x) + (a.y * a.y);
}

float dot(const sf::Vector2f& a, const sf::Vector2f& b) {
	return a.x * b.x + a.y * b.y;
}

float random(const float& min, const float& max) {
	return map(float(std::rand()), 0, 32767, min, max);
}

float constrain(const float& value, const float& min, const float& max) {
	return value > max ? max : value < min ? min : value;
}