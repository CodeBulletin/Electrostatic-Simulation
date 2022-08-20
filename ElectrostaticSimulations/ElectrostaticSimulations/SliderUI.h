#pragma once
#include "utility.h"

class SliderUI {
private:
	float min, max;
	float* value;
	bool isInt, isActive;
	sf::Vector2f pos, barpos;
	float w, barW, h;
	float Init;
	float (*func)(const float&);
	sf::Color BarColor, SliderColor, onClickColor;
	sf::Text Text, Textmin,Textmax,Textcurr;
	std::string text, textmin, textmax, textcurr;
	sf::Vertex V1, V2, V3, V4, VB1, VB2, VB3, VB4;
public:
	SliderUI() {}
	SliderUI(const sf::Vector2f& pos, const float& w, const float& h, const float& barW, const sf::Color& SliderColor, const sf::Color& BarColor, const sf::Color& onClickColor, const float& min, const float& max, const bool& isInt, float& value, float (*func)(const float&), const std::optional<float>& Init = {}) {
		this->min = min;
		this->max = max;
		this->isInt = isInt;
		this->Init = isInt?std::round(Init.value_or(value)):Init.value_or(value);
		this->value = &value;
		this->pos = pos;
		this->w = w;
		this->h = h;
		this->barW = barW;
		this->BarColor = BarColor;
		this->SliderColor = SliderColor;
		this->onClickColor = onClickColor;
		this->barpos = { map(this->Init, min, max, pos.x - w / 2, pos.x + w / 2), pos.y };

		isActive = false;

		V1.position = { pos.x - w / 2, pos.y - h / 4 };
		V1.color = SliderColor;
		V2.position = { pos.x + w / 2, pos.y - h / 4 };
		V2.color = SliderColor;
		V3.position = { pos.x + w / 2, pos.y + h / 4 };
		V3.color = SliderColor;
		V4.position = { pos.x - w / 2, pos.y + h / 4 };
		V4.color = SliderColor;

		VB1.position = { barpos.x - barW / 2, barpos.y - h / 2 };
		VB1.color = BarColor;
		VB2.position = { barpos.x + barW / 2, barpos.y - h / 2 };
		VB2.color = BarColor;
		VB3.position = { barpos.x + barW / 2, barpos.y + h / 2 };
		VB3.color = BarColor;
		VB4.position = { barpos.x - barW / 2, barpos.y + h / 2 };
		VB4.color = BarColor;
		this->func = func;
	}
	void setText(const std::string&, const std::vector<sf::Vector2f>&, const sf::Font&, const unsigned int&, const unsigned int&, const sf::Color&);
	void draw(sf::VertexArray&, std::vector<sf::Text>&);
	bool dragged(sf::Event::MouseMoveEvent&);
	void clicked(sf::Event::MouseButtonEvent&);
	void setActive(const bool&);
};