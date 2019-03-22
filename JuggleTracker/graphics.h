#pragma once
#include <SFML/Graphics.hpp>
#include <list>

namespace graphics
{
	enum ThingType
	{
		THING,
		BALL,
		JUGGLETHING,
		JUGGLEMESH
	};

	class Thing;
	class ThingManager;

	class Thing : public sf::Drawable
	{
	public:
		bool isDead() { return dead; };
		Thing() : manager(NULL), dead(false) {};
		
		virtual void update(const sf::View & view);
		virtual ThingType getType() const { return THING; };

	protected:
		ThingManager* manager;
		bool dead;

		
		virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const {};
	
	private:
		void setManager(ThingManager *_manager) { manager = _manager; };
		friend ThingManager;

	};

	class Ball : public Thing
	{
	public:
		Ball(sf::Vector2f pos, float radius = 50.f);
		~Ball();

		virtual void update(const sf::View & view);
		virtual ThingType getType() const { return BALL; };
	private:
		sf::Vector2f pos;
		virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
		sf::CircleShape circle;
		float radius;

	};

	class JuggleThing : public Thing
	{
	public:
		JuggleThing();
		~JuggleThing();

		void kill();
		void moveTo(float newX, float newY);

		virtual void update(const sf::View & view);
		virtual ThingType getType() const { return JUGGLETHING; };

		sf::Vector2f calcPos(const sf::View & view);

	private:
		sf::CircleShape circle;
		int counter;
		float x;
		float y;
		virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

		

	};

	class JuggleMesh : public Thing
	{
	public:
		JuggleMesh();
		
		virtual void update(const sf::View & view);
		virtual ThingType getType() const { return JUGGLEMESH; };

	private:
		sf::VertexArray lines;
		virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

	};

	class ThingManager : public sf::Drawable
	{
	public:
		ThingManager(sf::Mutex &_mutex) : myMutex(&_mutex) {};
		~ThingManager();
		
		void update(const sf::View & view);
		void add(Thing* thing);

		const std::list<Thing*>& getThings() const { return things; };

	private:
		sf::Mutex* myMutex;
		std::list<Thing*> things;
		virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
	};
}