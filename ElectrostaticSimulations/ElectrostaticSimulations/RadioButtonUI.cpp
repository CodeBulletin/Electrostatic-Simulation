#include "RadioButtonUI.h"

std::optional<size_t> RadioButtonUI::findIndex(int value) {
	for (size_t i = 0; i < totalButtons; i++) {
		if (values[i] == value) {
			return i;
		}
	}
	return {};
}

std::optional<size_t> RadioButtonUI::findIndex(bool value) {
	for (size_t i = 0; i < totalButtons; i++) {
		if (inBool[i] == value) {
			return i;
		}
	}
	return {};
}

void RadioButtonUI::draw(sf::VertexArray& vert, std::vector<sf::Text>& Texts) {
	for (CheckBoxUI& c : Boxes) {
		c.draw(vert, Texts);
	}
}

void RadioButtonUI::setTexts(const std::vector<std::string> texts, const sf::Font& font, const unsigned int& size, const sf::Color& col) {
	for (int i = 0; i < totalButtons; i++) {
		Boxes[i].setText(texts[i], font, size, col);
	}
}

void RadioButtonUI::clicked(sf::Event::MouseButtonEvent& mouse) {
	for (int i = 0; i < totalButtons; i++) {
		if (Boxes[i].clicked(mouse, true)) {
			for (int j = 0; j < totalButtons; j++) {
				inBool[j] = false;
			}
			inBool[i] = true;
			*value = values[i];
			break;
		}
	}
}