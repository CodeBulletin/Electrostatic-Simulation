#pragma once
#include "buttonUI.h"
#include "RadioButtonUI.h"
#include "SliderUI.h"
#include "buttonUI.h"

class UI {
private:
	sf::Vertex A, B, C, D;
	sf::VertexArray v;
	std::vector<buttonUI> Buttons;
	std::vector<CheckBoxUI> CheckBoxes;
	std::vector<RadioButtonUI> RadioButtons;
	std::vector<SliderUI> Sliders;
	std::vector<sf::Text> texts;
public:
	UI(const sf::Vector2f& a, const sf::Vector2f& c, const sf::Color& Col) {
		A.position = a;
		A.color = Col;
		B.position = { c.x, a.y };
		B.color = Col;
		C.position = c;
		C.color = Col;
		D.position = { a.x, c.y };
		D.color = Col;
	}
	std::vector<sf::Text>& getTexts();
	sf::VertexArray draw();
	bool clicked(sf::Event::MouseButtonEvent&);
	bool dragged(sf::Event::MouseMoveEvent&);
	void addButton(const buttonUI&);
	void addCheckBox(const CheckBoxUI&);
	void addRadioButton(const RadioButtonUI&);
	void addSlider(const SliderUI&);
	void update();
	void updatedragged();
	void clear();
	bool iscolliding(sf::Event::MouseMoveEvent&);
	bool iscolliding(sf::Event::MouseButtonEvent&);
};
