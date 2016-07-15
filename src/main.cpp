#include <SFML/Graphics.hpp>

#include "event.h"
#include "debug_text.h"
#include <vld.h>

sf::Font* g_defaultFont;
sf::Text g_debugText;
sf::RectangleShape g_debugTextBackground;

int main(int argc, char** argv)
{
	g_defaultFont = new sf::Font;

	g_defaultFont->loadFromFile("assets/fonts/m5x7.ttf");
	g_debugText.setFont(*g_defaultFont);
	g_debugTextBackground.setFillColor(sf::Color::Black);

	sf::RenderWindow window(sf::VideoMode(800, 600), "Astro---Tether", sf::Style::Default);
	window.setActive();

	sf::View uiView = window.getDefaultView();
	// Create the camera, origin at center
	const float w = 800; 
	const float h = 600;
	sf::View view(sf::FloatRect(-w / 2.0f, -h / 2.0f, w, h));

	sf::Clock clock;
	while (window.isOpen())
	{
		float dt = clock.restart().asSeconds();

		// Poll events
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
			{
				window.close();
			}

			if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
			{
				window.close();
			}

			if (event.type == sf::Event::KeyReleased)
			{
				//game.OnKeyReleased(event.key.code);
			}

			//if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Button::Left)
			//{
				// for now only allow one button to be triggered per frame
				//bool buttonTriggered = false;
				//const sf::Event::MouseButtonEvent &mbe = event.mouseButton;
				// check all the buttons on all the tilemaps to see if any buttons were pressed
				//for (auto itr = g_uiMaps.begin(); itr != g_uiMaps.end(); ++itr)
				//{
				//	auto buttons = itr->second->GetButtons();
				//	for (auto bitr = buttons.begin(); bitr != buttons.end(); ++bitr)
				//	{
				//		if ((*bitr)->CheckMousePress(mbe, window))
				//		{
				//			buttonTriggered = true;
				//			goto button_check_break;
				//		}
				//	}
				//}
			//	button_check_break:
				//if (!buttonTriggered)
				//{
				//	auto buttons = g_currentLevelMap->GetButtons();
				//	for (auto itr = buttons.begin(); itr != buttons.end(); ++itr)
				//	{
				//		if ((*itr)->CheckMousePress(mbe, window))
				//		{
				//			buttonTriggered = true;
				//			break;
				//		}
				//	}
				//}
			//}

			if (event.type == sf::Event::MouseWheelScrolled)
			{
				float zoom = 1.0f - event.mouseWheelScroll.delta * 0.11f;
				view.zoom(zoom);
			}
		}

		// Update actors

		// Clear
		window.clear(sf::Color(11, 21, 21));

		// Set active camera and  draw actors
		window.setView(view);
		//g_currentLevelMap->Draw(window);

		//for (auto itr = g_uiMaps.begin(); itr != g_uiMaps.end(); ++itr)
		//{
		//	uiView.setCenter(itr->second->GetWidth() / 2, itr->second->GetHeight() / 2);
		//	window.setView(uiView);
		//	itr->second->Draw(window);
		//}
		//window.setView(window.getDefaultView());
		//game.Draw(window);

		// Debug text
		window.setView(window.getDefaultView());
		window.draw(g_debugTextBackground);
		window.draw(g_debugText);

		DebugPrintf("Hello");

		// Display window
		window.display();
	}

	delete g_defaultFont;

	return 0;
}