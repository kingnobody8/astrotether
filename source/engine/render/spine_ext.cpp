#include "spinecpp/extension.h"
#include "SFML/Graphics.hpp"
#include "utility/types/types.h"

namespace spine
{
	void AtlasPage_disposeTexture(spine::Atlas::Page& page)
	{
		sf::RenderWindow* pRenWin = static_cast<sf::RenderWindow*>(page.rendererObject);
		int x = 0;
		x++;
	}

	void AtlasPage_createTexture(spine::Atlas::Page& page, const char* path)
	{
		sf::Texture* pTexture = new sf::Texture();
		pTexture->loadFromFile(path);

		page.rendererObject = pTexture;
	}

	
	std::string Util_readFile(const std::string& path)
	{
		TBuffer buff;
		buff.LoadFromFile(path);
	
		std::string ret;
		ret.assign((char*)buff.buffer, buff.size);
		return ret;
	}
}