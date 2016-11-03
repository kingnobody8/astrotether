#pragma once
#include "physics_image_ent.h"
#include "render/render_plugin.h"

namespace app
{
	namespace entity
	{

		PhysicsImageEnt::PhysicsImageEnt(const std::string& filepath, b2dJsonImage* pb2Image)
			: m_filepath(filepath)
			, m_pb2Image(pb2Image)
			, m_pBody(null)
			, m_pSprite(null)
		{
		}

		VIRTUAL PhysicsImageEnt::~PhysicsImageEnt()
		{
		}

		VIRTUAL void PhysicsImageEnt::Init()
		{
			sf::Texture* texture = new sf::Texture();
			if (!texture->loadFromFile(m_filepath))
				return;

			m_pSprite = new sf::Sprite(*texture);

			m_pBody = m_pb2Image->body;

			//calculate scale
			sf::Vector2f scale;
			imgSize.x = b2Distance(m_pb2Image->corners[0], m_pb2Image->corners[1]);
			imgSize.y = b2Distance(m_pb2Image->corners[1], m_pb2Image->corners[2]);
			sf::Vector2u texSize = m_pSprite->getTexture()->getSize();

			scale.x = imgSize.x / texSize.x;
			scale.y = imgSize.y / texSize.y;

			m_pSprite->setScale(scale);
			m_pSprite->setOrigin(texSize.x * 0.5f, texSize.y * 0.5f);
			//m_pSprite->setPosition(m_pb2Image->center.x - imgSize.x * 0.5f, m_pb2Image->center.y - imgSize.x * 0.5f);


			baka::render::RenderPlugin* pRenderPlug = static_cast<baka::render::RenderPlugin*>(baka::IPlugin::FindPlugin(baka::render::RenderPlugin::Type));
			//pRenderPlug->AddDrawable(m_pSprite);
		}

		VIRTUAL void PhysicsImageEnt::Exit()
		{
		}

		VIRTUAL void PhysicsImageEnt::Update(const sf::Time& dt)
		{
			if (m_pBody)
			{
				m_pSprite->setRotation(m_pBody->GetAngle() * -RAD_DEG);
				sf::Vector2f offsetScale(sin(m_pBody->GetAngle()), cos(m_pBody->GetAngle()));
				//m_pSprite->setPosition(m_pBody->GetPosition().x - imgSize.x * 0.5f * offsetScale.x, m_pBody->GetPosition().y - imgSize.y * 0.5f * offsetScale.y);
				m_pSprite->setPosition(m_pBody->GetPosition().x, -m_pBody->GetPosition().y);
			}
		}


	}
}