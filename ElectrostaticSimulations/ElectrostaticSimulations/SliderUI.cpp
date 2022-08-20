#include "SliderUI.h"

void SliderUI::setText(const std::string& text, const std::vector<sf::Vector2f>& textCoords, const sf::Font& font, const unsigned int& size, const unsigned int& size2, const sf::Color& textColor) {
	this->text = text;
	Text.setFont(font);
	Text.setString(text);
	Text.setOrigin(Text.getLocalBounds().width / 2.0f + size / 10, size / 2.0f + size / 10.0);
	Text.setPosition(textCoords[0]);
	Text.setCharacterSize(size);
	Text.setFillColor(textColor);
	this->textmin = std::to_string(int(func(min)));
	this->textmax = std::to_string(int(func(max)));
	Textmin.setFont(font);
	Textmax.setFont(font);
	Textmin.setString(textmin);
	Textmax.setString(textmax);
	Textmin.setOrigin(Text.getLocalBounds().width / 2.0f + size / 10, size / 2.0f + size / 10.0);
	Textmax.setOrigin(Text.getLocalBounds().width / 2.0f + size / 10, size / 2.0f + size / 10.0);
	Textmin.setPosition(textCoords[1]);
	Textmax.setPosition(textCoords[2]);
	Textmin.setCharacterSize(size2);
	Textmax.setCharacterSize(size2);
	Textmin.setFillColor(textColor);
	Textmax.setFillColor(textColor);
	Textcurr.setString(std::to_string(int(func(*value))));
	Textcurr.setFont(font);
	Textcurr.setOrigin(Text.getLocalBounds().width / 2.0f + size / 10, size / 2.0f + size / 10.0);
	Textcurr.setPosition(textCoords[3]);
	Textcurr.setCharacterSize(size2);
	Textcurr.setFillColor(textColor);
}

void SliderUI::draw(sf::VertexArray& vert, std::vector<sf::Text>& texts) {
	vert.append(V1);
	vert.append(V2);
	vert.append(V3);
	vert.append(V4);
	vert.append(VB1);
	vert.append(VB2);
	vert.append(VB3);
	vert.append(VB4);
	texts.push_back(Text);
	texts.push_back(Textmin);
	texts.push_back(Textmax);
	texts.push_back(Textcurr);
}

bool SliderUI::dragged(sf::Event::MouseMoveEvent& mouse) {
	if ((mouse.x > VB1.position.x && mouse.x < VB3.position.x && mouse.y > VB1.position.y && mouse.y < VB3.position.y) || isActive) {
		float val = map(constrain(mouse.x, pos.x - w / 2, pos.x + w / 2), pos.x - w / 2, pos.x + w / 2, min, max);
		val = isInt ? std::round(val) : val;
		barpos.x = map(val, min, max, pos.x - w / 2, pos.x + w / 2);
		VB1.position.x = barpos.x - barW / 2;
		VB1.color = onClickColor;
		VB2.position.x = barpos.x + barW / 2;
		VB2.color = onClickColor;
		VB3.position.x = barpos.x + barW / 2;
		VB3.color = onClickColor;
		VB4.position.x = barpos.x - barW / 2;
		VB4.color = onClickColor;
		*value = val;
		isActive = true;
		Textcurr.setString(std::to_string(int(func(val))));
		return true;
	}
	else {
		return false;
	}
}

void SliderUI::clicked(sf::Event::MouseButtonEvent& mouse) {
	if (mouse.x > V1.position.x && mouse.x < V3.position.x && mouse.y > V1.position.y && mouse.y < V3.position.y) {
		barpos.x = constrain(mouse.x, pos.x - w / 2, pos.x + w / 2);
		VB1.position.x = barpos.x - barW / 2;
		VB2.position.x = barpos.x + barW / 2;
		VB3.position.x = barpos.x + barW / 2;
		VB4.position.x = barpos.x - barW / 2;
		float val = map(barpos.x, pos.x - w / 2, pos.x + w / 2, min, max);
		val = isInt ? std::round(val) : val;
		Textcurr.setString(std::to_string(int(func(val))));
		*value = val;
	}
}

void SliderUI::setActive(const bool& b) {
	isActive = b;
	if (!b) {
		VB1.color = BarColor;
		VB2.color = BarColor;
		VB3.color = BarColor;
		VB4.color = BarColor;
	}
}