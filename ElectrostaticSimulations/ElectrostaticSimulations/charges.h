#pragma once
#include "utility.h"

class charge {
private:
	sf::Vector2f pos, vel, acc;
	std::vector<int> CollidedWith;
	float Charge, absCharge, ConstCharge;
	bool Mergeable, Moveable, doConstCharge, collideable;
	float Radius;
	float mass, fc;
	int Id;

public:
	charge() {}
	charge(sf::Vector2f&& pos, float Charge, float fc, bool Mergeable, bool Moveable, bool doConstCharge, bool collideable, float Radius, float mass, int& id, sf::Vector2f&& vel = { 0, 0 }) {
		this->pos = pos;
		this->Charge = Charge;
		absCharge = std::abs(Charge);
		this->Mergeable = Mergeable;
		this->Radius = Radius;
		this->vel = vel;
		this->Moveable = Moveable;
		this->collideable = collideable;
		acc = { 0.0f, 0.0f };
		this->mass = mass;
		this->doConstCharge = doConstCharge;
		this->fc = fc;
		if (doConstCharge) {
			ConstCharge = Charge;
		}
		Id = id++;
	}

	sf::Vector2f& getpos();
	void setpos(const sf::Vector2f&);
	sf::Vector2f& getvel();
	void setvel(const sf::Vector2f&);
	sf::Vector2f& getacc();
	void setacc(const sf::Vector2f&);

	float& getCharge();
	float& getRadius();

	void update(bool, int, int, int, int, float); // update position

	sf::CircleShape draw(bool); // return SFML Circle Shape which represnts our charge
	std::optional<std::vector<sf::VertexArray>> getElectricFieldLine(std::vector<charge>&, int, float, int); // returns array of electric field lines for the charge
	
	void applyforce(std::vector<charge>&); // apply forces on to the object
	void calforce(charge&); // calculate the forces

	void docol(std::vector<charge>&); // performs collision
	bool operator == (const charge&); // return true if two objects are same else false
	bool HasCollided(charge&); // return true if two objects has collided recently else false
	bool colliding(charge&); // return true if two objects are near engough to collide else false
	void elasticCol(charge&); // perform elastic collison
	void inelasticCol(charge&); // perform inelastic collison
};

sf::VertexArray ElectricFieldVector(std::vector<charge>&, float, int, int, int, int, bool, int);
sf::VertexArray prediction(charge, std::vector<charge>, float, int, bool, int, int, int, int);
