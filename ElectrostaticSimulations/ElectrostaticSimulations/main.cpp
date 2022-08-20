#include "files.h"

sf::RenderWindow window;

static const std::string name = "Electro static Simulation";
static float vecSize = 20.0f;
static int nl = 32;
static float speed = 100;
static float S = 2;
static float F = 2;
static float SimSpeed = 2;
static bool drawElectricField = true;
static bool drawElectricLines = true;
static bool boundary = true;
static int ColorArrow = 1, VoltageColor = 0;
static bool voltage = true;
static bool sim = false, simwas = false;
static float friction_co = 0.9f;
static float Charge = 10;
static float Radius = 10;
static float Mass = 1;
static bool Collision = false, IECollision = false, CCharge = false, Fixed = false, deleteMode = false;
static std::vector<charge> charges;
static charge tempCharge;
static sf::VertexArray tempV;
sf::Vector2f VEL;
int width, height, minwidth, minheight = 0;

void Reset();
void Exit();
void deletePrevoius();
void pause();
void save();
void load();
void saveImage();
float A(const float&);
float B(const float&);
float C(const float&);
float D(const float&);

void MakeUI(UI&, sf::Font&, sf::VertexArray&, sf::VertexArray&, sf::VertexArray&, sf::Text&, sf::Text&);

static bool shader1A = true, shader2A = true;

int main() {
	std::srand(std::time(0));
	int id = 0, frameCount = 0;

	float Volt, eField;

	sf::ContextSettings settings;
	settings.antialiasingLevel = 8;
	auto vid = sf::VideoMode::getDesktopMode();

	width = vid.width;
	height = vid.height;
	minwidth = width / 5;

	bool SliderActive = false;
	bool addActive = false;

	window.create(vid , name, sf::Style::Fullscreen, settings);
	sf::Event event;
	window.setVerticalSyncEnabled(true);

	if (!sf::Shader::isAvailable())
	{
		std::cout << "error Shaders are not Available on your GPU" << std::endl;
		std::cin.get();
	}

	sf::Shader shader, shader2;

	if (!shader.loadFromFile("voltage2.frag", sf::Shader::Fragment))
	{
		std::cout << "unable to load shader 'voltage2.frag' from files. Make sure its in the same folder where .exe file is located" << std::endl;
		shader1A = false;
	}
	if (!shader2.loadFromFile("voltage.frag", sf::Shader::Fragment))
	{
		std::cout << "unable to load shader 'voltage.frag' from files. Make sure its in the same folder where .exe file is located" << std::endl;
		shader2A = false;
	}

	sf::VertexArray v(sf::Quads, 4);
	v[0].position = { float(minwidth), 0.0f };
	v[1].position = { float(width), 0.0f };
	v[2].position = { float(width), float(height) };
	v[3].position = { float(minwidth), float(height) };

	shader.setUniform("dis", std::sqrt(dis2));
	shader.setUniform("res", float(height));

	shader2.setUniform("dis", std::sqrt(dis2));
	shader2.setUniform("res", float(height));



	//UI
	UI MainUI = UI(
		{ 0.0f, 0.0f },
		{ float(minwidth), float(height) },
		{100, 100, 100, 255});
	sf::Font font;
	if (!font.loadFromFile("Comfortaa-VariableFont_wght.ttf")) {
		std::cout << "unable to load 'Comfortaa-VariableFont_wght.ttf' font. Make sure it in the same folder as .exe file" << std::endl;
		std::cout << "you wont be able to see text :( font not found" << std::endl;
	}
	sf::VertexArray TB(sf::Quads, 8);
	sf::VertexArray VY(sf::Triangles, 3);
	sf::VertexArray VX(sf::Quads, 16);
	sf::Text VOLT, ELF;
	MakeUI(MainUI, font, TB, VY, VX, VOLT, ELF);
	//

	tempV = sf::VertexArray(sf::Lines, 6);

	while (window.isOpen()) {
		while (window.pollEvent(event)) {
			switch (event.type)
			{
			case sf::Event::MouseButtonPressed:
				frameCount = 1;
				if (event.mouseButton.button == sf::Mouse::Left) {
					if (!MainUI.clicked(event.mouseButton)) {
						if (!deleteMode) {
							charges.push_back(charge(sf::Vector2f(event.mouseButton.x, event.mouseButton.y), Charge, friction_co, IECollision, !Fixed, CCharge, Collision, Radius, Mass, id));
						}
						else {
							for (int j = 0; j <= charges.size(); j++) {
								if (std::sqrt(dist2(event.mouseButton.x, event.mouseButton.y, charges[j].getpos().x, charges[j].getpos().y)) <= charges[j].getRadius()) {
									charges.erase(std::next(charges.begin(), j));
									break;
								}
							}
						}
					}
				}
				break;
			case sf::Event::MouseMoved:
				if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
					if (MainUI.dragged(event.mouseMove)) {
						SliderActive = true;
					}
				}
				else if (SliderActive) {
					MainUI.updatedragged();
					SliderActive = false;
				}
				if (sf::Mouse::isButtonPressed(sf::Mouse::Right) && !addActive) {
					if (!MainUI.iscolliding(event.mouseMove)) {
						tempCharge = charge(sf::Vector2f(event.mouseMove.x, event.mouseMove.y), Charge, friction_co, IECollision, !Fixed, CCharge, Collision, Radius, Mass, id);
						addActive = true;
						if (simwas) {
							sim = false;
						}
					}
				}
				else if (sf::Mouse::isButtonPressed(sf::Mouse::Right) && addActive) {
					VEL = { tempCharge.getpos().x - event.mouseMove.x, tempCharge.getpos().y - event.mouseMove.y };
					VEL /= mag(VEL);
					VEL *= constrain(std::sqrtf(dist2(tempCharge.getpos(), { float(event.mouseMove.x), float(event.mouseMove.y) })), 0.0f, 400.0f) / 25.0f;
					float l2l1 = 2.0f / 10.0f;
					float sint = std::sinf(PI / 4);
					float cost = std::cosf(PI / 4);
					tempV[0].position = tempCharge.getpos() - VEL * 10.0f;
					tempV[1].position = tempCharge.getpos() + VEL * 10.0f;
					sf::Vector2f r = tempV[0].position - tempV[1].position;
					float a1 = r.x * cost;
					float b1 = r.y * sint;
					float a2 = r.x * sint;
					float b2 = r.y * cost;
					sf::Vector2f l1 = { l2l1 * (a1 + b1), l2l1 * (b2 - a2) };
					sf::Vector2f l2 = { l2l1 * (a1 - b1), l2l1 * (b2 + a2) };
					tempV[2].position = tempV[1].position + l1;
					tempV[3].position = tempV[1].position;
					tempV[4].position = tempV[1].position + l2;
					tempV[5].position = tempV[1].position;
					tempCharge.setvel(VEL);
				}
				break;
			case sf::Event::MouseButtonReleased:
				if (event.mouseButton.button == sf::Mouse::Right && addActive) {
					charges.push_back(tempCharge);
					tempCharge = charge();
					addActive = false;
					if (simwas) {
						sim = true;
					}
				}
				break;
			default:
				break;
			}
		}
		sf::Clock clock;
		float lastTime = 0;
		float fps = 144;
		clock.restart();
		nl = 2 * std::powl(2, int(F));
		vecSize = 10 * std::powf(2, S);
		speed = std::powf(10, SimSpeed);
		window.clear(sf::Color::Black);
		for (int i = charges.size() - 1; i >= 0; i--) {
			if (sim) {
				charges[i].docol(charges);
			}
		}
		int index = 0;
		for (charge& c : charges) {
			if (voltage) {
				std::string st = "loc[" + std::to_string(index) + "]";
				std::string st2 = "charge[" + std::to_string(index) + "]";
				if (VoltageColor) {
					shader.setUniform(st, c.getpos());
					shader.setUniform(st2, c.getCharge());
				}
				else {
					shader2.setUniform(st, c.getpos());
					shader2.setUniform(st2, c.getCharge());
				}
				index++;
			}
		}
		if (voltage) {
			if (VoltageColor && shader1A) {
				shader.setUniform("Size", int(charges.size()));
				window.draw(v, &shader);
			}
			else if(shader2A) {
				shader2.setUniform("Size", int(charges.size()));
				window.draw(v, &shader2);
			}
		}
		if (drawElectricField) {
			window.draw(ElectricFieldVector(charges, vecSize, ColorArrow, width, height, minwidth, voltage, VoltageColor));
		}
		for (charge& c : charges) {
			if (drawElectricLines) {
				auto v = c.getElectricFieldLine(charges, nl, 6, 1000);
				if (v) {
					for (sf::VertexArray& vv : *v) {
						window.draw(vv);
					}
				}
			}
		}
		Volt = 0;
		eField = 0;
		for (charge& c: charges) {
			if (sf::Mouse::getPosition(window).x > minwidth) {
				sf::Vector2f DIR = { c.getpos().x - sf::Mouse::getPosition(window).x, c.getpos().y - sf::Mouse::getPosition(window).y };
				float EE = kCOnst * c.getCharge() / (mag2(DIR) * dis2);
				eField += EE * 10000;
				Volt += EE * mag(DIR) * std::sqrtf(dis2);
			}
			window.draw(c.draw(VoltageColor));
			if (sim) {
				c.update(boundary, minwidth, minheight, width, height, speed/fps);
			}
		}
		for (charge &c: charges) {
			if (sim) {
				c.applyforce(charges);
			}
		}
		if (addActive) {
			window.draw(prediction(tempCharge, charges, speed / 144.0f, 10000, boundary, minwidth, minheight, width, height));
			window.draw(tempV);
			window.draw(tempCharge.draw(VoltageColor));
		}
		window.draw(MainUI.draw());
		for (sf::Text& t : MainUI.getTexts()) {
			window.draw(t);
		}
		if (frameCount % 20 == 0) {
			MainUI.update();
		}
		if (sim) {
			window.draw(VX);
		}
		else {
			window.draw(VY);
		}
		VOLT.setString(Volt > 10000 ? "V > 10000" : Volt < -10000 ? "V < -10000" : "V ~ " + std::to_string(int(Volt)));
		ELF.setString(eField > 1000.0 ? "E > 1000" : eField < -1000.0 ? "E < 1000" : "E ~ " + std::to_string(int(eField)));
		window.draw(TB);
		window.draw(VOLT);
		window.draw(ELF);
		window.display();
		frameCount++;
		float currentTime = clock.getElapsedTime().asSeconds();
		fps = 1.f / (currentTime - lastTime);
		lastTime = currentTime;
	}
	return 0;
}





void Reset() {
	charges.clear();
}

void Exit() {
	exit(0);
}

void deletePrevoius() {
	if (charges.size() > 0) {
		charges.pop_back();
	}
}

void pause() {
	sim = !sim;
	simwas = sim;
}

float A(const float& a) {
	return 2 * std::powl(2, int(a));
}

float B(const float& b) {
	return 10 * std::powf(2, b);
}

float C(const float& c) {
	return std::powf(10, c);
}

float D(const float& d) {
	return d;
}

void MakeUI(UI& mainUI, sf::Font& font, sf::VertexArray& TB, sf::VertexArray& VY, sf::VertexArray& VX, sf::Text& VOLT, sf::Text& ELF) {
	buttonUI b;
	CheckBoxUI cb;
	RadioButtonUI rb;
	SliderUI s;

	b = buttonUI({ 70.0f, 30.0f }, 120, 30, sf::Color::White, { 255, 255, 255, 125 }, Reset);
	b.setText("RESET", font, 30, sf::Color::Black);
	mainUI.addButton(b);
	b = buttonUI({ minwidth - 70.0f, 30 }, 120, 30, sf::Color::White, { 255, 255, 255, 125 }, Exit);
	b.setText("EXIT", font, 30, sf::Color::Black);
	mainUI.addButton(b);
	b = buttonUI({ minwidth / 2.0f, 30 }, 30, 30, sf::Color::White, { 255, 255, 255, 125 }, pause);
	b.setText(" ", font, 30, sf::Color::Black);
	mainUI.addButton(b);
	cb = CheckBoxUI({ 20.f, 80.0f }, 20, 2, sf::Color::Black, sf::Color::White, sf::Color::Red, drawElectricLines);
	cb.setText("Electric Field Lines", font, 20, sf::Color::White);
	mainUI.addCheckBox(cb);
	s = SliderUI({ 260.0f, 112.0f }, 150, 20, 10, sf::Color::Black, sf::Color::White, { 125, 125, 125, 255 }, 0, 4, true, F, A, {});
	s.setText("Electric Field Lines:", { { 170.0f, 110.0f }, { 245.0f, 115.0f }, { 415.0f, 115.0f }, { 330.0f, 132.0f } }, font, 15.0f, 10.0f, sf::Color::White);
	mainUI.addSlider(s);
	cb = CheckBoxUI({ 20.0f, 160 }, 20, 2, sf::Color::Black, sf::Color::White, sf::Color::Red, drawElectricField);
	cb.setText("Electric Field Vector", font, 20, sf::Color::White);
	mainUI.addCheckBox(cb);
	s = SliderUI({ 270.0f, 192.0f }, 150, 20, 10, sf::Color::Black, sf::Color::White, { 125, 125, 125, 255 }, 4, 0, true, S, B, {});
	s.setText("Electric Field Vector:", { { 170.0f, 190.0f }, { 250.0f, 195.0f }, { 430.0f, 195.0f }, { 343.0f, 212.0f } }, font, 15.0f, 10.0f, sf::Color::White);
	mainUI.addSlider(s);
	rb = RadioButtonUI(3, { 0, 1, 2 }, { {20.0f, 230.0f}, {150.0f, 230.0f}, {280.0f, 230.0f} }, 15, 2, sf::Color::Black, sf::Color::White, sf::Color::Blue, ColorArrow);
	rb.setTexts({ "No Color", "Color", "Black & white" }, font, 10, sf::Color::White);
	mainUI.addRadioButton(rb);
	cb = CheckBoxUI({ 20.0f, 280.0f }, 20, 2, sf::Color::Black, sf::Color::White, sf::Color::Red, voltage);
	cb.setText("Voltage", font, 20, sf::Color::White);
	mainUI.addCheckBox(cb);
	rb = RadioButtonUI(2, { 0, 1 }, { {20.0f, 310.0f}, {150.0f, 310.0f} }, 15, 2, sf::Color::Black, sf::Color::White, sf::Color::Blue, VoltageColor);
	rb.setTexts({ "Red & blue", "Green" }, font, 10, sf::Color::White);
	mainUI.addRadioButton(rb);

	s = SliderUI({ minwidth / 2.0f, 420.0f }, minwidth - 100.0f, 40, 20, sf::Color::Black, sf::Color::White, { 125, 125, 125, 255 }, 0, 2, true, SimSpeed, C, {});
	s.setText("Simulation speed: ", { { minwidth / 2.0f + 25.0f, 370.0f }, { 150.0f, 419.0f }, { 465.0f, 419.0f }, { minwidth / 2.0f + 109, 455.0f } }, font, 25.0f, 25.0f, sf::Color::White);
	mainUI.addSlider(s);

	s = SliderUI({ minwidth / 2.0f, 550.0f }, minwidth - 200.0f, 20, 10, sf::Color::Black, sf::Color::White, { 125, 125, 125, 255 }, -100, 100, true, Charge, D, {});
	s.setText("Charge:", { { minwidth / 2.0f + 25.0f, 520.0f }, { 300 - minwidth / 2.0f, 552.0f }, { 142 + minwidth / 2.0f, 552.0f }, { minwidth / 2.0f + 37.0f, 570.0f } }, font, 20.0f, 15.0f, sf::Color::White);
	mainUI.addSlider(s);
	s = SliderUI({ minwidth / 2.0f, 625.0f }, minwidth - 200.0f, 20, 10, sf::Color::Black, sf::Color::White, { 125, 125, 125, 255 }, 1, 40, true, Radius, D, {});
	s.setText("Radius:", { { minwidth / 2.0f + 25.0f, 595.0f }, { 315 - minwidth / 2.0f, 627.0f }, { 140 + minwidth / 2.0f, 627.0f }, { minwidth / 2.0f + 37.0f, 645.0f } }, font, 20.0f, 15.0f, sf::Color::White);
	mainUI.addSlider(s);
	s = SliderUI({ minwidth / 2.0f, 700.0f }, minwidth - 200.0f, 20, 10, sf::Color::Black, sf::Color::White, { 125, 125, 125, 255 }, 1, 10, true, Mass, D, {});
	s.setText("Mass:", { { minwidth / 2.0f + 20.0f, 670.0f }, { 307 - minwidth / 2.0f, 702.0f }, { 130 + minwidth / 2.0f, 702.0f }, { minwidth / 2.0f + 37.0f, 720.0f } }, font, 20.0f, 15.0f, sf::Color::White);
	mainUI.addSlider(s);

	cb = CheckBoxUI({ 20.0f, 770.0f }, 20, 2, sf::Color::Black, sf::Color::White, sf::Color::Red, Collision);
	cb.setText("Collision", font, 20, sf::Color::White);
	mainUI.addCheckBox(cb);

	cb = CheckBoxUI({ 20.0f, 820.0f }, 20, 2, sf::Color::Black, sf::Color::White, sf::Color::Red, IECollision);
	cb.setText("Inelastic Collision", font, 20, sf::Color::White);
	mainUI.addCheckBox(cb);

	cb = CheckBoxUI({ 20.0f, 870.0f }, 20, 2, sf::Color::Black, sf::Color::White, sf::Color::Red, CCharge);
	cb.setText("Constant Charge", font, 20, sf::Color::White);
	mainUI.addCheckBox(cb);

	cb = CheckBoxUI({ 20.0f, 920.0f }, 20, 2, sf::Color::Black, sf::Color::White, sf::Color::Red, Fixed);
	cb.setText("Fixed Charge", font, 20, sf::Color::White);
	mainUI.addCheckBox(cb);

	cb = CheckBoxUI({ 20.0f, 970.0f }, 20, 2, sf::Color::Black, sf::Color::White, sf::Color::Red, deleteMode);
	cb.setText("Delete Mode", font, 20, sf::Color::White);
	mainUI.addCheckBox(cb);

	b = buttonUI({ (minwidth - 100.0f) / 2.0f, 1000.0f }, 150, 20, sf::Color::White, { 255, 255, 255, 125 }, deletePrevoius);
	b.setText("Delete Last", font, 20, sf::Color::Black);
	mainUI.addButton(b);

	b = buttonUI({ (minwidth - 300.0f) / 2.0f, 1040.0f }, 60, 20, sf::Color::White, { 255, 255, 255, 125 }, save);
	b.setText("save", font, 20, sf::Color::Black);
	mainUI.addButton(b);
	b = buttonUI({ (minwidth - 150.0f) / 2.0f, 1040.0f }, 60, 20, sf::Color::White, { 255, 255, 255, 125 }, load);
	b.setText("load", font, 20, sf::Color::Black);
	mainUI.addButton(b);
	b = buttonUI({ (minwidth - 100.0f), 1050.0f }, 130, 20, sf::Color::White, { 255, 255, 255, 125 }, saveImage);
	b.setText("Save Image", font, 20, sf::Color::Black);
	mainUI.addButton(b);

	VX[0].position = { minwidth / 2.0f - 10.0f, 20.0f };
	VX[0].color = sf::Color::Black;
	VX[1].position = { minwidth / 2.0f - 2.0f, 20.0f };
	VX[1].color = sf::Color::Black;
	VX[2].position = { minwidth / 2.0f - 2.0f, 40.0f };
	VX[2].color = sf::Color::Black;
	VX[3].position = { minwidth / 2.0f - 10.0f, 40.0f };
	VX[3].color = sf::Color::Black;
	VX[4].position = { minwidth / 2.0f + 2.0f, 20.0f };
	VX[4].color = sf::Color::Black;
	VX[5].position = { minwidth / 2.0f + 10.0f, 20.0f };
	VX[5].color = sf::Color::Black;
	VX[6].position = { minwidth / 2.0f + 10.0f, 40.0f };
	VX[6].color = sf::Color::Black;
	VX[7].position = { minwidth / 2.0f + 2.0f, 40.0f };
	VX[7].color = sf::Color::Black;
	VY[0].position = { minwidth / 2.0f - 10.0f, 20.0f };
	VY[0].color = sf::Color::Black;
	VY[1].position = { minwidth / 2.0f + 10.0f, 30.0f };
	VY[1].color = sf::Color::Black;
	VY[2].position = { minwidth / 2.0f - 10.0f, 40.0f };
	VY[2].color = sf::Color::Black;
	TB[0].position = { minwidth - 110.0f, height - 120.0f };
	TB[0].color = sf::Color::White;
	TB[1].position = { minwidth - 23.0f, height - 120.0f };
	TB[1].color = sf::Color::White;
	TB[2].position = { minwidth - 23.0f, height - 53.0f };
	TB[2].color = sf::Color::White;
	TB[3].position = { minwidth - 110.0f, height - 53.0f };
	TB[3].color = sf::Color::White;
	TB[4].position = { minwidth - 108.0f, height - 118.0f };
	TB[4].color = { 50, 50, 50, 255 };
	TB[5].position = { minwidth - 25.0f, height - 118.0f };
	TB[5].color = { 50, 50, 50, 255 };
	TB[6].position = { minwidth - 25.0f, height - 55.0f };
	TB[6].color = { 50, 50, 50, 255 };
	TB[7].position = { minwidth - 108.0f, height - 55.0f };
	TB[7].color = { 50, 50, 50, 255 };

	VOLT.setFont(font);
	VOLT.setOrigin(VOLT.getLocalBounds().width / 2.0f + 1.5f, 9.0f);
	VOLT.setCharacterSize(15.0f);
	VOLT.setFillColor(sf::Color::White);
	VOLT.setPosition({ minwidth - 100.0f, height - 100.0f });
	ELF.setFont(font);
	ELF.setOrigin(VOLT.getLocalBounds().width / 2.0f + 1.5f, 9.0f);
	ELF.setCharacterSize(15.0f);
	ELF.setFillColor(sf::Color::White);
	ELF.setPosition({ minwidth - 100.0f, height - 70.0f });
}

void save() {
	window.setVisible(false);
	save(charges);
	window.setVisible(true);
}

void load() {
	window.setVisible(false);
	load(charges);
	window.setVisible(true);
}

void saveImage() {
	window.setVisible(false);
	saveImage(window);
	window.setVisible(true);
}