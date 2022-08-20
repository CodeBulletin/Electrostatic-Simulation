#include "charges.h"

sf::Vector2f& charge::getpos() {
	return pos;
}

void charge::setpos(const sf::Vector2f& pos) {
	this->pos = pos;
}

sf::Vector2f& charge::getvel() {
	return vel;
}

void charge::setvel(const sf::Vector2f& vel) {
	this->vel = vel;
}

sf::Vector2f& charge::getacc() {
	return acc;
}

void charge::setacc(const sf::Vector2f& acc) {
	this->acc = acc;
}

float& charge::getCharge() {
	return Charge;
}

float& charge::getRadius() {
	return Radius;
}

bool charge::HasCollided(charge& other) {
	return CollidedWith.empty() ? false : std::find(CollidedWith.begin(), CollidedWith.end(), other.Id) != CollidedWith.end() ? true : false;
}

bool charge::colliding(charge& other) {
	return std::sqrt(dist2(other.pos, pos)) <= (Radius + other.Radius) + 1 && this->collideable && other.collideable;
}

bool charge::operator == (const charge& other) {
	return Id == other.Id;
}

void charge::update(bool remain_on_screen, int minwidth, int minheight, int width, int height, float dt) {
	if (Moveable) {
		sf::Vector2f previous_pos = pos;
		pos += vel * dt + std::powf(dt, 2) * acc;
		vel = (pos - previous_pos) / dt;
	}
	acc = { 0, 0 };
	CollidedWith.clear();
	if (remain_on_screen) {
		if (pos.x - Radius <= minwidth) {
			vel.x = -vel.x;
			pos.x = minwidth + Radius + 1;
			vel = vel * fc;
		}
		else if (pos.x + Radius >= width) {
			vel.x = -vel.x;
			pos.x = width - Radius - 1;
			vel = vel * fc;
		}
		if (pos.y - Radius <= minheight) {
			vel.y = -vel.y;
			pos.y = minheight + Radius + 1;
			vel = vel * fc;
		}
		else if (pos.y + Radius >= height) {
			vel.y = -vel.y;
			pos.y = height - Radius - 1;
			vel = vel * fc;
		}
	}
}

void charge::applyforce(std::vector<charge>& others) {
	for (charge&c : others) {
		if (c.Id != this->Id) {
			calforce(c);
		}
	}
}

void charge::docol(std::vector<charge>& others) {
	for (int i = others.size() - 1; i >= 0; i--) {
		if (Id != others[i].Id && collideable && others[i].collideable) {
			if (colliding(others[i])) {
				if (!(Mergeable && others[i].Mergeable)) {
					elasticCol(others[i]);
				}
				else {
					inelasticCol(others[i]);
					others.erase(std::next(others.begin(), i));
					i--;
				}
			}
		}
	}
}

void charge::calforce(charge& other) {
	sf::Vector2f dir = (other.pos - pos);
	dir /= mag(dir);
	float r2 = dist2(other.pos, pos) * dis2;
	float force = kCOnst * Charge * other.Charge / r2;
	sf::Vector2f forcevec = -dir * force;
	acc += forcevec / mass;
}

void charge::elasticCol(charge& other) {
	if (!HasCollided(other)) {
		if (Moveable && other.Moveable) {
			float c = (2 * other.mass / (other.mass + mass)) * (dot(vel - other.vel, pos - other.pos) / mag2(pos - other.pos));
			float oc = (2 * mass / (other.mass + mass)) * (dot(other.vel - vel, other.pos - pos) / mag2(other.pos - pos));
			other.vel = (other.vel - oc * (other.pos - pos));
			vel = (vel - c * (pos - other.pos));
		}
		else if (Moveable && !other.Moveable) {
			float c = 2 * (dot(vel, pos - other.pos) / mag2(pos - other.pos));
			vel = (vel - c * (pos - other.pos));
		}
		else if (!Moveable && other.Moveable) {
			float oc = 2 * (dot(other.vel, other.pos - pos) / mag2(other.pos - pos));
			other.vel = (other.vel - oc * (other.pos - pos));
		}
		other.CollidedWith.push_back(Id);
		CollidedWith.push_back(other.Id);
		if (!doConstCharge && !other.doConstCharge) {
			float ch = (Charge + other.Charge);
			Charge = ch / 2;
			absCharge = std::abs(Charge);
			other.Charge = ch / 2;
			other.absCharge = std::abs(other.Charge);
		}
		else if (doConstCharge && !other.doConstCharge){
			other.Charge = Charge;
			other.absCharge = std::abs(other.Charge);
		}
		else if (!doConstCharge && other.doConstCharge) {
			Charge = other.Charge;
			absCharge = std::abs(Charge);
		}
	}
}

void charge::inelasticCol(charge& other) {
	if (Moveable && other.Moveable) {
		vel = (mass * vel + other.mass * other.vel) / (mass + other.mass);
		pos = (mass * pos + other.mass * other.pos) / (mass + other.mass);
		acc = (mass * acc + other.mass * other.acc) / (mass + other.mass);
	}
	else if (Moveable && !other.Moveable) {
		vel = { 0, 0 };
		pos = other.pos;
		Moveable = false;
	}
	else if (!Moveable && other.Moveable) {
		vel = { 0, 0 };
	}
	else if (!Moveable && !other.Moveable) {
		pos = (mass * pos + other.mass * other.pos) / (mass + other.mass);
		vel = { 0, 0 };
	}
	if ((doConstCharge && other.doConstCharge) || ((!doConstCharge && !other.doConstCharge))) {
		Charge += other.Charge;
	}
	else if (other.doConstCharge) {
		Charge = other.Charge;
	}
	absCharge = std::abs(Charge);
	mass += other.mass;
	Radius = std::sqrt(Radius * Radius + other.Radius * Radius);
}

sf::CircleShape charge::draw(bool Voltage) {
	sf::CircleShape cs;
	cs.setRadius(Radius);
	cs.setOrigin(Radius, Radius);
	cs.setPosition(pos.x, pos.y);
	if (!Voltage) {
		if (Charge < 0.0f) {
			cs.setFillColor(sf::Color(0, 0, 255));
		}
		else if (Charge == 0.0f) {
			cs.setFillColor(sf::Color(255, 255, 255));
		}
		else {
			cs.setFillColor(sf::Color(255, 0, 0));
		}
	}
	else {
		if (Charge < 0.0f) {
			cs.setFillColor(sf::Color(255, 0, 255));
		}
		else if (Charge == 0.0f) {
			cs.setFillColor(sf::Color(0, 125, 0));
		}
		else {
			cs.setFillColor(sf::Color(0, 255, 0));
		}
	}
	return cs;
}

std::optional <std::vector<sf::VertexArray>> charge::getElectricFieldLine(std::vector<charge>& Charges, int nl, float scale, int dis) {
	if (!Charge == 0.0f) {
		std::vector<sf::VertexArray> v(nl, sf::VertexArray(sf::LineStrip, dis));
		int j = 0;
		sf::Vector2f dir;
		sf::Vector2f netE;
		sf::Vector2f pt;
		float r2, e, mE;
		float incr = (int)(2 * PI / nl * 1000.0) / 1000.0;
		for (float a = 0; a < 2 * PI - 0.001*nl; a += incr) {
			pt = { pos.x + Radius, pos.y };
			pt = rotatePoint(pt, pos, a);
			for (int i = 0; i < dis; i++) {
				v[j][i].position = { pt.x, pt.y };
				netE = { 0, 0 };
				for (charge& c : Charges) {
					if (c.Charge != 0) {
						r2 = (c.pos.x - pt.x) * (c.pos.x - pt.x) + (c.pos.y - pt.y) * (c.pos.y - pt.y);
						e = kCOnst * c.Charge / (r2 * dis2);
						if (Charge > 0) {
							dir = pt - c.pos;
						}
						else {
							dir = c.pos - pt;
						}
						netE += dir * e;
					}
				}
				mE = mag(netE);
				netE /= mE / scale;
				pt += netE;
			}
			j++;
		}
		return v;
	}
	else {
		return {};
	}
}

sf::VertexArray ElectricFieldVector(std::vector<charge>& Charges, float vecSize, int Color, int width, int height, int minwidth, bool V, int VoltageCol) {//to improve
	int Rows = int(height / vecSize);
	int Cols = int((width - minwidth + vecSize) / vecSize);
	sf::VertexArray v(sf::Lines, Rows * Cols * 6);
	float scale = 2 * vecSize / 3;
	float l2l1 = 3.0f / 10.0f;
	float sint = std::sinf(PI / 4);
	float cost = std::cosf(PI / 4);
	for (int i = 0; i < Rows; i++) {
		for (int j = 0; j < Cols; j++) {
			sf::Vector2f pos = { j * vecSize + vecSize / 2 + minwidth, i * vecSize + vecSize / 2 };
			sf::Vector2f netE = { 0, 0 };
			float E = 0;
			float absE = 0;
			for (charge& c : Charges) {
				float r2 = (c.getpos().x - pos.x) * (c.getpos().x - pos.x) + (c.getpos().y - pos.y) * (c.getpos().y - pos.y);
				float e = kCOnst * c.getCharge() / (r2 * dis2);
				sf::Vector2f dir = pos - c.getpos();
				netE += dir * e;
				E += e;
				absE += std::abs(e);
			}
			sf::Vector2f to = (netE / mag(netE)) * scale + pos;
			sf::Vector2f r = pos - to;
			float a1 = r.x * cost;
			float b1 = r.y * sint;
			float a2 = r.x * sint;
			float b2 = r.y * cost;
			sf::Vector2f l1 = { l2l1 * (a1 + b1), l2l1 * (b2 - a2) };
			sf::Vector2f l2 = { l2l1 * (a1 - b1), l2l1 * (b2 + a2) };
			int index = (i + j * Rows) * 6;
			v[index + 0].position = pos;
			v[index + 1].position = to;
			v[index + 2].position = to + l1;
			v[index + 3].position = to;
			v[index + 4].position = to + l2;
			v[index + 5].position = to;
			if (E < 0 && Color == 1 && !V) {
				for (int k = 0; k < 6; k++) {
					v[index + k].color = sf::Color(0, 0, E < -0.001f ? 255 : E >= 0 ? 0 : map(E, 0, -0.001f, 0, 255), 255);
				}
			}
			else if (E > 0 && Color == 1 && !V) {
				for (int k = 0; k < 6; k++) {
					v[index + k].color = sf::Color(E > 0.001f ? 255 : E <= 0 ? 0 : map(E, 0, 0.001f, 0, 255), 0, 0, 255);
				}
			}
			if (E < 0 && Color == 1 && V && !VoltageCol) {
				for (int k = 0; k < 6; k++) {
					v[index + k].color = sf::Color(0, 0, E < -0.001f ? 255 : E >= 0 ? 0 : map(E, 0, -0.001f, 0, 255), 255);
				}
			}
			else if (E > 0 && Color == 1 && V && !VoltageCol) {
				for (int k = 0; k < 6; k++) {
					v[index + k].color = sf::Color(E > 0.001f ? 255 : E <= 0 ? 0 : map(E, 0, 0.001f, 0, 255), 0, 0, 255);
				}
			}
			else if (E < 0 && Color == 1 && VoltageCol && V) {
				for (int k = 0; k < 6; k++) {
					v[index + k].color = sf::Color(0, E < -0.001f ? 255 : E >= 0 ? 0 : map(E, -0.001f, 0, 255, 0), 0, 255);
				}
			}
			else if (E > 0 && Color == 1 && VoltageCol && V) {
				for (int k = 0; k < 6; k++) {
					v[index + k].color = sf::Color(E > 0.001f ? 255 : E <= 0 ? 0 : map(E, 0.001f, 0, 255, 0), 0, E > 0.001f ? 255 : E <= 0 ? 0 : map(E, 0.001f, 0, 255, 0), 255);
				}
			}
			else if (Color == 2) {
				for (int k = 0; k < 6; k++) {
					v[index + k].color = sf::Color(255, 255, 255, absE > 0.0005f ? 255 : absE <= 0 ? 0 : map(absE, 0, 0.0005f, 0, 255));
				}
			}
		}
	}
	return v;
}

sf::VertexArray prediction(charge temp, std::vector<charge> charges, float dt, int size, bool remain_on_screen, int minwidth, int minheight, int width, int height) {
	charges.push_back(temp);
	sf::VertexArray v(sf::LinesStrip, size);
	for (int i = 0; i < size; i++) {
		v[i].position = charges[charges.size() - 1].getpos();
		for (int j = 0; j < charges.size(); j++) {
			charges[j].update(remain_on_screen, minwidth, minheight, width, height, dt);
		}
		for (int j = 0; j < charges.size(); j++) {
			charges[j].applyforce(charges);
		}
	}
	return v;
}