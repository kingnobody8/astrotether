#include "box2d-sfml.h"
#include "utility/helper/func.h"
#include "spine/include/spinecpp/extension.h"

namespace baka
{
	namespace render
	{
		__todo() //there is a lot happening here i don't understand, like why do i have to invert both the y axis and the rotation, also why does the image not render offset correctly

		PhysicsDrawable::PhysicsDrawable(const b2dJsonImage& jImage, const std::string& path)
			: m_vVertexArray(new sf::VertexArray(sf::Triangles, jImage.numIndices))
			, m_pTexture(null)
			, m_pBody(null)
			, m_jImage(jImage)
			, m_path(path)
		{
			m_pTexture = new sf::Texture();
			if (!m_pTexture->loadFromFile(path + jImage.file))
				assert(false);

			m_pBody = jImage.body;

			m_offsetTransform.translate(jImage.center.x, -jImage.center.y);
			m_offsetTransform.rotate(jImage.angle * -RAD_DEG);

			//create verticies
			for (int i = 0; i < m_vVertexArray->getVertexCount(); ++i)
			{
				sf::Vertex& vert = m_vVertexArray->operator[](i);
				int index = jImage.indices[i];
				vert.position.x = jImage.points[index * 2];
				vert.position.y = jImage.points[index * 2 + 1];

				vert.texCoords.x = jImage.uvCoords[index * 2] * m_pTexture->getSize().x;
				vert.texCoords.y = jImage.uvCoords[index * 2 + 1] * m_pTexture->getSize().y;
			}
		}

		PhysicsDrawable::~PhysicsDrawable()
		{
			delete m_vVertexArray;
			delete m_pTexture;
			m_jImage = b2dJsonImage();
		}

		VIRTUAL void PhysicsDrawable::draw(sf::RenderTarget& target, sf::RenderStates states) const
		{
			sf::Transform bodyTransform;
			if (m_pBody)
			{
				bodyTransform.translate(m_pBody->GetPosition().x, -m_pBody->GetPosition().y);
				bodyTransform.rotate(m_pBody->GetAngle() * -RAD_DEG);
			}

			states.transform *= bodyTransform * m_offsetTransform;
			states.texture = m_pTexture;
			target.draw(*m_vVertexArray, states);
		}

	}
}