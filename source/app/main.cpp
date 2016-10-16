
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

#include "rapidjson/FileReadStream.h"
#include <cstdio>

#include "time/util_time.h"
#include "resource/json.h"


int main()
{
	FILE* fp = fopen("assets/init.json", "rb"); // non-Windows use "r"
	char readBuffer[65536];
	rapidjson::FileReadStream is(fp, readBuffer, sizeof(readBuffer));
	rapidjson::Document d;
	d.ParseStream(is);
	fclose(fp);

	util::JSON initJson(d);
	std::string asdf = initJson.Read();

	std::string appName = initJson["title"].GetString();
	util::vec2 screenSize(initJson["size"]["x"].GetDouble(), initJson["size"]["y"].GetDouble());
	double viewRatio = initJson["view_ratio"].GetDouble();
	sf::Color backgroundColor;
	backgroundColor.r = initJson["background_color"]["r"].GetInt();
	backgroundColor.g = initJson["background_color"]["g"].GetInt();
	backgroundColor.b = initJson["background_color"]["b"].GetInt();

	srand(unsigned int(util::Time::GetTimeSinceEpoch().Milli()));

	// Create the main window
	sf::RenderWindow window(sf::VideoMode(screenSize.x, screenSize.y), appName.c_str(), sf::Style::Default);

	// Create the camera, origin at center
	const float vw = screenSize.x * viewRatio;
	const float vh = screenSize.y * viewRatio;
	sf::View view(sf::FloatRect(-vw / 2.0f, -vh / 2.0f, vw, vh));
	window.setView(view);

	// Make it the active window for OpenGL calls
	window.setActive();

	// Create a clock for measuring the time elapsed
	sf::Clock clock;

	// Start the game loop
	while (window.isOpen())
	{
		sf::Time delta = clock.restart();

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
		}


		window.setView(view);
		window.clear(backgroundColor);

		//for (size_t i = 0; i < vActors.size(); ++i)
		//{
		//	vActors[i]->Draw(window);
		//}


		//m_pWorld->DrawDebugData();
		//m_debugDraw.Flush();

		// Finally, display the rendered frame on screen
		window.display();
	}

	return 0;
}
