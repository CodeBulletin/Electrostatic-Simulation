#include "buttonUI.h"

void buttonUI::draw(sf::VertexArray& v, std::vector<sf::Text>& t) {
	v.append(V1);
	v.append(V2);
	v.append(V3);
	v.append(V4);
	t.push_back(Text);
}

void buttonUI::setText(const std::string& text, const sf::Font& f, const unsigned int& size, const sf::Color& c) {
	this->text = text;
	Text.setFont(f);
	Text.setString(text);
	Text.setCharacterSize(size);
	Text.setOrigin(Text.getLocalBounds().width / 2.0f + size/10, size/2.0f + size/10.0);
	Text.setPosition(pos);
	Text.setFillColor(c);
}

void buttonUI::clicked(sf::Event::MouseButtonEvent& mouse) {
	if (mouse.x > V1.position.x && mouse.x < V3.position.x && mouse.y > V1.position.y && mouse.y < V3.position.y) {
		V1.color = onClickColor;
		V2.color = onClickColor;
		V3.color = onClickColor;
		V4.color = onClickColor;
		func();
	}
}

void buttonUI::update() {
	V1.color = Color;
	V2.color = Color;
	V3.color = Color;
	V4.color = Color;
}
