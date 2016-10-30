#pragma once
#include "Box2D/Box2D.h"
#include <SFML/Graphics.hpp>

namespace baka
{
	struct RenderVerts
	{
		void Vertex(sf::Vector2f v, const sf::Color& c)
		{
			if (m_count == e_maxVertices)
				Flush();

			v.y *= -1;

			m_vertArray[m_count].position = v;
			m_vertArray[m_count].color = c;
			++m_count;
		}

		void Flush()
		{
			m_pRenWin->draw(m_vertArray, m_count, m_type);
			m_count = 0;
		}

		enum { e_maxVertices = 3 * 512 };
		sf::Vertex m_vertArray[e_maxVertices];
		int32 m_count = 0;
		sf::PrimitiveType m_type = sf::PrimitiveType::Lines;
		sf::RenderWindow* m_pRenWin = nullptr;
	};

		// This class implements debug drawing callbacks that are invoked inside b2World::Step.
		class DebugDraw : public b2Draw
		{
		public:
			DebugDraw();
			~DebugDraw();

			void Init();
			void Exit();

			void DrawPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color);

			void DrawSolidPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color);

			void DrawCircle(const b2Vec2& center, float32 radius, const b2Color& color);

			void DrawSolidCircle(const b2Vec2& center, float32 radius, const b2Vec2& axis, const b2Color& color);

			void DrawSegment(const b2Vec2& p1, const b2Vec2& p2, const b2Color& color);

			void DrawTransform(const b2Transform& xf);

			void DrawPoint(const b2Vec2& p, float32 size, const b2Color& color);

			void DrawAABB(b2AABB* aabb, const b2Color& color);

			void DrawParticles(const b2Vec2 *centers, const b2Vec2 *velocities, float32 radius, const b2ParticleColor *colors, int32 count);

			void Flush();

			void SetRenderWindow(sf::RenderWindow* pRenWin);

		private:
			RenderVerts m_points;
			RenderVerts m_lines;
			RenderVerts m_triangles;
		};
}


