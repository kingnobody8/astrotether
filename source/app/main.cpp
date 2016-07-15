
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

#include "box2d.h"
#include "engine/b2djson/b2djson.h"
#include "engine.h"

const int VELOCITY_ITERATIONS = 8;
const int POSITION_ITERATIONS = 3;
const b2Vec2 GRAVITY = b2Vec2(0.0f, 0.0f);

int main()
{
	engine::Engine* engi = engine::Engine::Get();

	engi->Init(null);

#ifndef IS_IOS
	while (!engi->GetQuit())
	{
		engi->RunFrame(null);
	}
	engi->Exit();
#endif
	return 0;

	//b2dJson m_json;

	//b2World* m_pWorld = new b2World(GRAVITY);
	//m_pWorld->SetAllowSleeping(true);
	//m_pWorld->SetWarmStarting(true);
	//m_pWorld->SetContinuousPhysics(true);
	//m_pWorld->SetSubStepping(false);

	//std::string errorMsg;
	//m_json.readFromFile((std::string("assets/ship.json")).c_str(), errorMsg, m_pWorld);

	//// Create the main window
	//sf::RenderWindow window(sf::VideoMode(640, 480), "SFML window with OpenGL", sf::Style::Default);

	//// Make it the active window for OpenGL calls
	//window.setActive();

	//sf::IntRect camMoveRect;
	//camMoveRect.left = 400.0f;
	//camMoveRect.height = 0.25f;
	//camMoveRect.width = window.getSize().x;
	//camMoveRect.height = window.getSize().y;

	//// Create a clock for measuring the time elapsed
	//sf::Clock clock;

	//// Start the game loop
	//while (window.isOpen())
	//{
	//	sf::Time delta = clock.getElapsedTime();

	//	m_pWorld->Step(delta.asSeconds(), VELOCITY_ITERATIONS, POSITION_ITERATIONS);

	//	// Process events
	//	sf::Event event;
	//	while (window.pollEvent(event))
	//	{
	//		// Close window: exit
	//		if (event.type == sf::Event::Closed)
	//			window.close();

	//		// Escape key: exit
	//		if ((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::Escape))
	//			window.close();

	//		// Resize event: adjust the viewport
	//		//if (event.type == sf::Event::Resized)
	//			//glViewport(0, 0, event.size.width, event.size.height);
	//	}

	//	sf::VertexArray verts;
	//	verts.resize(5);
	//	verts[0].position.x = camMoveRect.left;
	//	verts[0].position.y = camMoveRect.top;
	//	verts[1].position.x = camMoveRect.left + camMoveRect.width;
	//	verts[1].position.y = camMoveRect.top;
	//	verts[2].position.x = camMoveRect.left + camMoveRect.width;
	//	verts[2].position.y = camMoveRect.top + camMoveRect.height;
	//	verts[3].position.x = camMoveRect.left;
	//	verts[3].position.y = camMoveRect.top + camMoveRect.height;
	//	verts[4].position.x = camMoveRect.left;
	//	verts[4].position.y = camMoveRect.top;

	//	for (size_t i = 0; i < verts.getVertexCount(); ++i)
	//	{
	//		verts[i].color = sf::Color::Blue;
	//	}

	//	window.draw(&verts[0], verts.getVertexCount(), sf::PrimitiveType::LinesStrip);


	//	// Finally, display the rendered frame on screen
	//	window.display();
	//}

	//return 0;
}
