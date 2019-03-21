#include <iostream>
#include <SFML/System/Lock.hpp>

#include "graphics.h"

namespace graphics
{
	Ball::Ball(sf::Vector2f pos, float radius) : Thing(), pos(pos), radius(radius)
	{
		circle = sf::CircleShape(radius, 100);
		circle.setFillColor(sf::Color::Green);
		circle.setOrigin(radius / 2, radius / 2);
		circle.setPosition(pos);
	}

	Ball::~Ball()
	{
		std::cout << "ball is dead" << std::endl;
	}

	void Ball::update()
	{
		radius = radius - 0.5f;

		if (radius <= 0)
		{
			dead = true;
		}
		else
		{
			circle.setOrigin(radius, radius);
			circle.setRadius(radius);
		}

	}

	void Ball::draw(sf::RenderTarget & target, sf::RenderStates states) const
	{
		target.draw(circle, states);
	}

	void Thing::update()
	{
		dead = true;
	}

	ThingManager::~ThingManager()
	{
		sf::Lock lock(*myMutex);

		for (Thing *i : things) delete i;
		things.clear();
	}
	void ThingManager::update()
	{
		sf::Lock lock(*myMutex);

		// Update all objects
		for (Thing *i : things) i->update();

		{
			// Delete all dead objects
			auto it = things.begin();
			while (it != things.end())
			{
				if ((*it)->isDead())
				{
					delete *it;
					it = things.erase(it);
				}
				else
				{
					it++;
				}
			}
		}


	}
	void ThingManager::add(Thing * thing)
	{
		sf::Lock lock(*myMutex);

		thing->setManager(this);
		things.push_back(thing);
	}

	void ThingManager::draw(sf::RenderTarget & target, sf::RenderStates states) const
	{
		sf::Lock lock(*myMutex);

		for (Thing *i : things) target.draw(*i, states);
	}

	JuggleThing::JuggleThing() : x(0), y(0), Thing()
	{
		circle = sf::CircleShape(30.f, 50);
		circle.setFillColor(sf::Color::Green);
		circle.setOrigin(circle.getRadius(), circle.getRadius());
	}

	JuggleThing::~JuggleThing()
	{
		std::cout << "This jugglething is dead" << std::endl;
	}

	void JuggleThing::kill()
	{
		dead = true;
	}

	void JuggleThing::moveTo(float newX, float newY)
	{
		x = newX;
		y = newY;
	}

	void JuggleThing::update()
	{
		std::cout << "x: " << x << "\t y: " << y << std::endl;
	}
	void JuggleThing::draw(sf::RenderTarget & target, sf::RenderStates states) const
	{
		float drawX = x * target.getSize().x;
		float drawY = y * target.getSize().y;

		sf::CircleShape circle2(circle);
		circle2.setPosition(drawX, drawY);
		target.draw(circle2, states);
	}
}