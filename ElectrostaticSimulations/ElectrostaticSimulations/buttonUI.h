#pragma once
#include "utility.h"

class buttonUI {
private:
	sf::Vector2f pos;
	sf::Vertex V1, V2, V3, V4;
	sf::Text Text;
	sf::Color onClickColor, Color;
	float w, h;
	void (*func) ();
	std::string text;

public:
	buttonUI() {}
	buttonUI(const sf::Vector2f& pos, const float& w, const float& h, const sf::Color& Color, const sf::Color& onClickColor, void (*func)()) {
		this->pos = pos;
		this->w = w;
		this->h = h;
		this->func = func;
		this->onClickColor = onClickColor;
		this->Color = Color;
		V1.position = { pos.x - w / 2, pos.y - h / 2 };
		V1.color = Color;
		V2.position = { pos.x + w / 2, pos.y - h / 2 };
		V2.color = Color;
		V3.position = { pos.x + w / 2, pos.y + h / 2 };
		V3.color = Color;
		V4.position = { pos.x - w / 2, pos.y + h / 2 };
		V4.color = Color;
	}
	void setText(const std::string&, const sf::Font&, const unsigned int&, const sf::Color&);
	void draw(sf::VertexArray&, std::vector<sf::Text>&);
	void clicked(sf::Event::MouseButtonEvent&);
	void update();
};