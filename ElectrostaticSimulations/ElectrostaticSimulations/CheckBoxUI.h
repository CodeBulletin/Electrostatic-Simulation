#pragma once
#include "utility.h"

class CheckBoxUI {
private:
	sf::Vector2f pos;
	sf::Vertex V1, V2, V3, V4, VO1, VO2, VO3, VO4, VI1, VI2, VI3, VI4;
	sf::Text Text;
	sf::Color fillColor, outlineColor, Color;
	std::string text;
	float size, outlineSize;
	bool* value;
public:
	CheckBoxUI() {}
	CheckBoxUI(const sf::Vector2f& pos, const float& size, const float& outlineSize, const sf::Color& Color, const sf::Color& outlineColor, const sf::Color& fillColor, bool& value) {
		this->pos = pos;
		this->size = size;
		this->Color = Color;
		this->outlineColor = outlineColor;
		this->fillColor = fillColor;
		this->value = &value;

		V1.position = { pos.x - size / 2.0f, pos.y - size / 2.0f };
		V1.color = Color;
		V2.position = { pos.x + size / 2.0f, pos.y - size / 2.0f };
		V2.color = Color;
		V3.position = { pos.x + size / 2.0f, pos.y + size / 2.0f };
		V3.color = Color;
		V4.position = { pos.x - size / 2.0f, pos.y + size / 2.0f };
		V4.color = Color;

		VO1.position = { pos.x - size / 2.0f - outlineSize, pos.y - size / 2.0f - outlineSize };
		VO1.color = outlineColor;
		VO2.position = { pos.x + size / 2.0f + outlineSize, pos.y - size / 2.0f - outlineSize };
		VO2.color = outlineColor;
		VO3.position = { pos.x + size / 2.0f + outlineSize, pos.y + size / 2.0f + outlineSize };
		VO3.color = outlineColor;
		VO4.position = { pos.x - size / 2.0f - outlineSize, pos.y + size / 2.0f + outlineSize };
		VO4.color = outlineColor;

		VI1.position = { pos.x - size / 2.0f + size / 8.0f, pos.y - size / 2.0f + size / 8.0f };
		VI1.color = fillColor;
		VI2.position = { pos.x + size / 2.0f - size / 8.0f, pos.y - size / 2.0f + size / 8.0f };
		VI2.color = fillColor;
		VI3.position = { pos.x + size / 2.0f - size / 8.0f, pos.y + size / 2.0f - size / 8.0f };
		VI3.color = fillColor;
		VI4.position = { pos.x - size / 2.0f + size / 8.0f, pos.y + size / 2.0f - size / 8.0f };
		VI4.color = fillColor;
	}
	void setText(const std::string&, const sf::Font&, const unsigned int&, const sf::Color&);
	void clicked(sf::Event::MouseButtonEvent&);
	bool clicked(sf::Event::MouseButtonEvent&, const bool&);
	void draw(sf::VertexArray&, std::vector<sf::Text>&);
};
