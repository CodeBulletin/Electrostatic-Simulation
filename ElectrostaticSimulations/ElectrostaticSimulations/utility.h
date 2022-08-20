#pragma once

#include <iostream>
#include <fstream>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <optional>
#include <iterator>
#include <SFML/Graphics.hpp>


#define PI 3.14159265358979323846f
#define dis2 10000000000.0f
#define kCOnst 8987551792.3f

sf::Vector2f rotatePoint(const sf::Vector2f& point, const sf::Vector2f& center, const float& angle);

float map(const float& value, const float& min, const float& max, const float& minRange, const float& maxRange);

float dist2(const sf::Vector2f& a, const sf::Vector2f& b);
float dist2(const float& x, const float& y, const float& x2, const float& y2);

float mag(const sf::Vector2f& a);

float mag2(const sf::Vector2f& a);

float dot(const sf::Vector2f& a, const sf::Vector2f& b);

float random(const float& min, const float& max);

float constrain(const float& value, const float& min, const float& max);

bool loadShader(sf::Shader&, std::string);