
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

#include "box2d.h"
#include "engine/b2djson/b2djson.h"
#include "engine/physics/debug_draw.h"

const int VELOCITY_ITERATIONS = 8;
const int POSITION_ITERATIONS = 3;
const b2Vec2 GRAVITY = b2Vec2(0.0f, 0.0f);

const std::string APP_NAME("Astro---Tether");
const sf::Vector2i SCREEN_SIZE(1280, 720);
const float VIEW_RATIO = 0.1f;

int main()
{
	engine::RenderVerts v;
	//v.m_type = sf::PrimitiveType::Triangles;

	b2dJson m_json;

	b2World* m_pWorld = new b2World(GRAVITY);
	m_pWorld->SetAllowSleeping(true);
	m_pWorld->SetWarmStarting(true);
	m_pWorld->SetContinuousPhysics(true);
	m_pWorld->SetSubStepping(false);

	std::string errorMsg;
	m_json.readFromFile((std::string("assets/ship.json")).c_str(), errorMsg, m_pWorld);

	b2Body* pShip = m_json.getBodyByName("ship");

	// Create the main window
	sf::RenderWindow window(sf::VideoMode(SCREEN_SIZE.x, SCREEN_SIZE.y), APP_NAME.c_str(), sf::Style::Default);

	v.m_pRenWin = &window;

	engine::DebugDraw m_debugDraw;
	m_debugDraw.SetRenderWindow(&window);

	sf::View uiView = window.getDefaultView();
	// Create the camera, origin at center
	const float vw = SCREEN_SIZE.x * VIEW_RATIO;
	const float vh = SCREEN_SIZE.y * VIEW_RATIO;
	sf::View view(sf::FloatRect(-vw / 2.0f, -vh / 2.0f, vw, vh));
	window.setView(view);

	m_pWorld->SetDebugDraw(&m_debugDraw);
	uint32 flags = 0;
	flags += true * b2Draw::e_shapeBit;
	flags += true * b2Draw::e_jointBit;
	flags += false * b2Draw::e_aabbBit;
	flags += false * b2Draw::e_centerOfMassBit;
	flags += true * b2Draw::e_particleBit;
	m_debugDraw.SetFlags(flags);

	// Make it the active window for OpenGL calls
	window.setActive();

	// Create a clock for measuring the time elapsed
	sf::Clock clock;

	float tot = 0;

	// Start the game loop
	while (window.isOpen())
	{
		sf::Time delta = clock.getElapsedTime();
		clock.restart();

		float secs = delta.asSeconds();
		tot += secs;
		m_pWorld->Step(secs, VELOCITY_ITERATIONS, POSITION_ITERATIONS);

		// Process events
		sf::Event event;
		while (window.pollEvent(event))
		{
			// Close window: exit
			if (event.type == sf::Event::Closed)
				window.close();

			// Escape key: exit
			if ((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::Escape))
				window.close();

			
			if (event.type == sf::Event::MouseWheelScrolled)
			{
				float zoom = 1.0f - event.mouseWheelScroll.delta * 0.11f;
				view.zoom(zoom);
			}

			// Resize event: adjust the viewport
			//if (event.type == sf::Event::Resized)
				//glViewport(0, 0, event.size.width, event.size.height);
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left))
		{
			pShip->SetTransform(pShip->GetPosition(), pShip->GetAngle() + 10 * secs);
			pShip->SetAngularVelocity(0);
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right))
		{
			pShip->SetTransform(pShip->GetPosition(), pShip->GetAngle() - 10 * secs);
			pShip->SetAngularVelocity(0);
		}

		//Get ship's direction
		float angle = pShip->GetAngle();
		angle += 3.14159f / 2.0f;
		b2Vec2 dir(cos(angle), sin(angle));
		dir.Normalize();

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up))
		{
			pShip->ApplyForceToCenter(dir * 1000000 * secs, true);
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down))
		{
			pShip->ApplyForceToCenter(-dir * 1000000 * secs, true);
		}
		

		window.setView(view);

		window.clear(sf::Color(11,31,31));


		//v.Vertex(sf::Vector2f(0, 0), sf::Color::Green);
		//v.Vertex(sf::Vector2f(window.getSize().x, window.getSize().y), sf::Color::Blue);
		//v.Vertex(sf::Vector2f(window.getSize().x / 2 * sin(tot), window.getSize().y), sf::Color::Red);
		//v.Flush();

		v.Vertex(sf::Vector2f(pShip->GetPosition().x, pShip->GetPosition().y), sf::Color::Red);
		v.Vertex(sf::Vector2f(pShip->GetPosition().x + dir.x * 10, pShip->GetPosition().y + dir.y * 10), sf::Color::Red);
		v.Flush();

		m_pWorld->DrawDebugData();
		m_debugDraw.Flush();


		// Finally, display the rendered frame on screen
		window.display();
	}

	return 0;
}
