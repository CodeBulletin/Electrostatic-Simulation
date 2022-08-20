#include "TextInputUI.h"

void TextInputUI::draw(sf::VertexArray& V, std::vector<sf::Text>& t) {
	if (active) {
		for (int i = 0; i < 12; i++) {
			V.append(v[i]);
		}
		t.push_back(Text);
	}
}

void TextInputUI::toggle() {
	active = true;
}