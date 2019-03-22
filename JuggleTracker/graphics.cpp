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

	void Ball::update(const sf::View & view)
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

	void Thing::update(const sf::View & view)
	{
		dead = true;
	}

	ThingManager::~ThingManager()
	{
		sf::Lock lock(*myMutex);

		for (Thing *i : things) delete i;
		things.clear();
	}
	void ThingManager::update(const sf::View & view)
	{
		sf::Lock lock(*myMutex);

		// Update all objects
		for (Thing *i : things) i->update(view);

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
		circle.setOutlineColor(sf::Color::White);
		circle.setOutlineThickness(2);

		circle.setFillColor(sf::Color::Transparent);
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

	void JuggleThing::update(const sf::View & view)
	{
		
		// std::cout << "x: " << x << "\t y: " << y << std::endl;

		circle.setPosition(calcPos(view));
	}
	
	void JuggleThing::draw(sf::RenderTarget & target, sf::RenderStates states) const
	{		
		target.draw(circle, states);
	}

	JuggleMesh::JuggleMesh() : lines(sf::PrimitiveType::Lines)
	{
	}

	void JuggleMesh::update(const sf::View & view)
	{
		lines.clear();

		if (manager == NULL)
			return;

		for (auto itr1 = manager->getThings().begin(); itr1 != manager->getThings().end(); itr1++)
		{
			if ((*itr1)->getType() != JUGGLETHING)
				continue;

			JuggleThing * thing1 = (JuggleThing*)*itr1;
			sf::Vertex vertex1(thing1->calcPos(view), sf::Color::White);

			for (auto itr2 = manager->getThings().begin(); itr2 != itr1; itr2++)
			{
				if ((*itr2)->getType() != JUGGLETHING)
					continue;

				JuggleThing * thing2 = (JuggleThing*)*itr2;				
				sf::Vertex vertex2(thing2->calcPos(view), sf::Color::White);

				lines.append(vertex1);
				lines.append(vertex2);
			}
		}
	}

	void JuggleMesh::draw(sf::RenderTarget & target, sf::RenderStates states) const
	{

		target.draw(lines, states);
	}

	sf::Vector2f JuggleThing::calcPos(const sf::View & view)
	{
		float drawX = -x * view.getSize().x + view.getCenter().x; // -x voor spiegelbeeld
		float drawY = y * view.getSize().y + view.getCenter().y;

		return sf::Vector2f(drawX, drawY);
	}
}