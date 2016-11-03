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
			imgSize.x = b2Distance(m_pb2Image->corners[0], m_pb2Image->corners[1]);
			imgSize.y = b2Distance(m_pb2Image->corners[1], m_pb2Image->corners[2]);
			sf::Vector2u texSize = m_pSprite->getTexture()->getSize();
			sf::Vector2f scale(imgSize.x / texSize.x, imgSize.y / texSize.y);

			m_pSprite->setScale(scale);

			offset.rotate(m_pb2Image->angle);
			offset.translate(m_pb2Image->center.x / scale.x, m_pb2Image->center.y / scale.y);

			//m_pSprite->setOrigin(imgSize.x * 0.5f, imgSize.y * 0.5f);
			m_pSprite->setOrigin(texSize.x * 0.5f, texSize.y * 0.5f);
		//	m_pSprite->setOrigin(texSize.x * 0.5f - m_pb2Image->center.x /*/ scale.x*/, texSize.y * 0.5f + /*m_pb2Image->center.y / scale.y*/);
			//m_pSprite->setPosition(m_pb2Image->center.x - imgSize.x * 0.5f, m_pb2Image->center.y - imgSize.x * 0.5f);
			angleOffset = m_pb2Image->angle;
			posOffset.x = m_pb2Image->center.x;
			posOffset.y = m_pb2Image->center.y;

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
				sf::Transform temp;
				temp.rotate(m_pBody->GetAngle() * -RAD_DEG);
				temp.translate(m_pBody->GetPosition().x, m_pBody->GetPosition().y);

				sf::Transform output = temp * offset;

				float rotation = m_pBody->GetAngle() * -RAD_DEG + angleOffset * -RAD_DEG;

				sf::Vector2f po;
				po.x = cos(m_pBody->GetAngle() + angleOffset);
				po.y = sin(m_pBody->GetAngle() + angleOffset);
				po.x = po.x * posOffset.Length();
				po.y = po.y * posOffset.Length();
				//po.x = po.y = 0;

				m_pSprite->setRotation( rotation);
				//m_pSprite->setPosition(m_pBody->GetPosition().x - imgSize.x * 0.5f * offsetScale.x, m_pBody->GetPosition().y - imgSize.y * 0.5f * offsetScale.y);
				b2Vec2 pos = m_pBody->GetPosition();
				m_pSprite->setPosition(m_pBody->GetPosition().x + po.x, -m_pBody->GetPosition().y - po.y);
			}
		}


	}
}