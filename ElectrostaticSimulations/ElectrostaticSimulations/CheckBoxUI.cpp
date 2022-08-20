#include "CheckBoxUI.h"

void CheckBoxUI::setText(const std::string& text, const sf::Font& f, const unsigned int& size, const sf::Color& c) {
	this->text = text;
	Text.setFont(f);
	Text.setString(text);
	Text.setOrigin(0.0f, size / 2.0f + size / 10.0);
	Text.setPosition({ pos.x + this->size + size / 2.0f, pos.y });
	Text.setCharacterSize(size);
	Text.setFillColor(c);
}

void CheckBoxUI::clicked(sf::Event::MouseButtonEvent& mouse) {
	if (mouse.x > V1.position.x && mouse.x < V3.position.x && mouse.y > V1.position.y && mouse.y < V3.position.y) {
		*value = !*value;
	}
}

bool CheckBoxUI::clicked(sf::Event::MouseButtonEvent& mouse, const bool&) {
	if (mouse.x > V1.position.x && mouse.x < V3.position.x && mouse.y > V1.position.y && mouse.y < V3.position.y) {
		return true;
	}
	return false;
}

void CheckBoxUI::draw(sf::VertexArray& v, std::vector<sf::Text>& t) {
	v.append(VO1);
	v.append(VO2);
	v.append(VO3);
	v.append(VO4);
	v.append(V1);
	v.append(V2);
	v.append(V3);
	v.append(V4);
	if (*value) {
		v.append(VI1);
		v.append(VI2);
		v.append(VI3);
		v.append(VI4);
	}
	t.push_back(Text);
}