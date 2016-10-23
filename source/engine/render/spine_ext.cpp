#include "spinecpp/extension.h"
#include "SFML/Graphics.hpp"
#include "utility/types/types.h"

namespace spine
{
	void AtlasPage_disposeTexture(spine::Atlas::Page& page)
	{
		delete (sf::Texture*)page.rendererObject;
	}

	void AtlasPage_createTexture(spine::Atlas::Page& page, const char* path)
	{
		sf::Texture* texture = new sf::Texture();
		if (!texture->loadFromFile(path)) return;

		if (page.magFilter == spine::Atlas::Filter::Linear) texture->setSmooth(true);
		if (page.uWrap == spine::Atlas::Wrap::Repeat && page.vWrap == spine::Atlas::Wrap::Repeat) texture->setRepeated(true);

		page.rendererObject = texture;
		sf::Vector2u size = texture->getSize();
		page.width = size.x;
		page.height = size.y;
	}

	
	std::string Util_readFile(const std::string& path)
	{
		TBuffer buff;
		buff.LoadFromFile(path);
	
		std::string ret;
		ret.assign((char*)buff.buffer, buff.size);
		buff.clear();
		return ret;
	}
}