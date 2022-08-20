#pragma once
#include "CheckBoxUI.h"

class RadioButtonUI {
private:
	std::vector<CheckBoxUI> Boxes;
	bool* inBool;
	std::vector<int> values;
	std::vector<sf::Vector2f> pos;
	std::vector<std::string> text;
	sf::Color fillColor, outlineColor, Color;
	float size, outlineSize;
	int totalButtons;
	bool test;
	int* value;
public:
	RadioButtonUI() {}
	RadioButtonUI(const int& totalButtons, const std::vector<int>& values, const std::vector<sf::Vector2f>& pos, const float& size, const float& outlineSize, const sf::Color& Color, const sf::Color& outlineColor, const sf::Color& fillColor, int& value) {
		this->totalButtons = totalButtons;
		Boxes = std::vector<CheckBoxUI>(totalButtons);
		inBool = new bool[totalButtons];
		this->pos = pos;
		this->values = values;
		this->size = size;
		this->outlineSize = outlineSize;
		this->fillColor = fillColor;
		this->outlineColor = outlineColor;
		this->Color = Color;
		for (int i = 0; i < totalButtons; i++) {
			inBool[i] = false;
			Boxes[i] = CheckBoxUI({ pos[i].x, pos[i].y }, size, outlineSize, Color, outlineColor, fillColor, inBool[i]);
		}
		auto trueIndex = findIndex(value);
		if (trueIndex) {
			inBool[*trueIndex] = true;
		}
		this->value = &value;
	}
	std::optional<size_t> findIndex(int value);
	std::optional<size_t> findIndex(bool value);
	void clicked(sf::Event::MouseButtonEvent&);
	void draw(sf::VertexArray&, std::vector<sf::Text>&);
	void setTexts(const std::vector<std::string> texts, const sf::Font&, const unsigned int&, const sf::Color&);
};