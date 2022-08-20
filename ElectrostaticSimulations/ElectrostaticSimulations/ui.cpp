#include "ui.h"

sf::VertexArray UI::draw() {
	v = sf::VertexArray();
	texts.clear();
	v.append(A);
	v.append(B);
	v.append(C);
	v.append(D);
	for (buttonUI& b : Buttons) {
		b.draw(v, texts);
	}
	for (CheckBoxUI& cb : CheckBoxes) {
		cb.draw(v, texts);
	}
	for (RadioButtonUI& rb : RadioButtons) {
		rb.draw(v, texts);
	}
	for (SliderUI& s : Sliders) {
		s.draw(v, texts);
	}
	v.setPrimitiveType(sf::Quads);
	return v;
}

std::vector<sf::Text>& UI::getTexts() {
	return texts;
}

void UI::addButton(const buttonUI& b) {
	Buttons.push_back(b);
}

void UI::addCheckBox(const CheckBoxUI& cb) {
	CheckBoxes.push_back(cb);
}

void UI::addRadioButton(const RadioButtonUI& rb) {
	RadioButtons.push_back(rb);
}

void UI::addSlider(const SliderUI& s) {
	Sliders.push_back(s);
}

void UI::update() {
	for (buttonUI& b : Buttons) {
		b.update();
	}
}

void UI::updatedragged() {
	for (SliderUI& b : Sliders) {
		b.setActive(false);
	}
}

bool UI::clicked(sf::Event::MouseButtonEvent& mouse) {
	if (iscolliding(mouse)) {
		for (buttonUI& b : Buttons) {
			b.clicked(mouse);
		}
		for (CheckBoxUI& cb : CheckBoxes) {
			cb.clicked(mouse);
		}
		for (RadioButtonUI& rb : RadioButtons) {
			rb.clicked(mouse);
		}
		for (SliderUI& s : Sliders) {
			s.clicked(mouse);
		}
		return true;
	}
	else {
		return false;
	}
}

bool UI::dragged(sf::Event::MouseMoveEvent& mouse) {
	if (iscolliding(mouse)) {
		for (SliderUI& s : Sliders) {
			if (s.dragged(mouse)) {
				break;
			}
		}
		return true;
	}
	else {
		return false;
	}
}

void UI::clear() {
	Buttons.clear();
	CheckBoxes.clear();
	RadioButtons.clear();
	Sliders.clear();
}

bool UI::iscolliding(sf::Event::MouseMoveEvent& mouse) {
	return mouse.x >= A.position.x && mouse.x <= C.position.x && mouse.y >= A.position.y && mouse.y <= C.position.y;
}

bool UI::iscolliding(sf::Event::MouseButtonEvent& mouse) {
	return mouse.x >= A.position.x && mouse.x <= C.position.x && mouse.y >= A.position.y && mouse.y <= C.position.y;
}