
#include "debug_draw.h"

namespace engine
{
	sf::Color B2ColorConvert(const b2Color& c)
	{
		return sf::Color(c.r * 255, c.g * 255, c.b * 255, c.a * 255);
	}

	DebugDraw::DebugDraw()
	{
	}

	DebugDraw::~DebugDraw()
	{
	}

	void DebugDraw::Create()
	{
	}

	void DebugDraw::Destroy()
	{
	}

	void DebugDraw::DrawPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& c)
	{
		sf::Color clr = B2ColorConvert(c);

		b2Vec2 p1 = vertices[vertexCount - 1];
		for (int32 i = 0; i < vertexCount; ++i)
		{
			b2Vec2 p2 = vertices[i];
			m_lines.Vertex(sf::Vector2f(p1.x, p1.y), clr);
			m_lines.Vertex(sf::Vector2f(p2.x, p2.y), clr);
			p1 = p2;
		}
	}

	void DebugDraw::DrawSolidPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& c)
	{
		b2Color tmpFillColor(0.5f * c.r, 0.5f * c.g, 0.5f * c.b, 0.5f);
		sf::Color clr = B2ColorConvert(c);
		sf::Color fillClr = B2ColorConvert(tmpFillColor);
		
		for (int32 i = 1; i < vertexCount - 1; ++i)
		{
			b2Vec2 a = vertices[0];
			b2Vec2 b = vertices[i];
			b2Vec2 c = vertices[i + 1];

			m_triangles.Vertex(sf::Vector2f(a.x, a.y), fillClr);
			m_triangles.Vertex(sf::Vector2f(b.x, b.y), fillClr);
			m_triangles.Vertex(sf::Vector2f(c.x, c.y), fillClr);
		}

		b2Vec2 p1 = vertices[vertexCount - 1];
		for (int32 i = 0; i < vertexCount; ++i)
		{
			b2Vec2 p2 = vertices[i];
			m_lines.Vertex(sf::Vector2f(p1.x ,p1.y), clr);
			m_lines.Vertex(sf::Vector2f(p2.x, p2.y), clr);
			p1 = p2;
		}
	}

	void DebugDraw::DrawCircle(const b2Vec2& center, float32 radius, const b2Color& color)
	{
		const float32 k_segments = 16.0f;
		const float32 k_increment = 2.0f * b2_pi / k_segments;
		float32 sinInc = sinf(k_increment);
		float32 cosInc = cosf(k_increment);
		b2Vec2 r1(1.0f, 0.0f);
		b2Vec2 v1 = center + radius * r1;
		sf::Color clr = B2ColorConvert(color);
		for (int32 i = 0; i < k_segments; ++i)
		{
			// Perform rotation to avoid additional trigonometry.
			b2Vec2 r2;
			r2.x = cosInc * r1.x - sinInc * r1.y;
			r2.y = sinInc * r1.x + cosInc * r1.y;
			b2Vec2 v2 = center + radius * r2;
			m_lines.Vertex(sf::Vector2f(v1.x, v1.y), clr);
			m_lines.Vertex(sf::Vector2f(v2.x, v2.y), clr);
			r1 = r2;
			v1 = v2;
		}
	}

	void DebugDraw::DrawSolidCircle(const b2Vec2& center, float32 radius, const b2Vec2& axis, const b2Color& color)
	{
		const float32 k_segments = 16.0f;
		const float32 k_increment = 2.0f * b2_pi / k_segments;
		float32 sinInc = sinf(k_increment);
		float32 cosInc = cosf(k_increment);
		b2Vec2 v0 = center;
		b2Vec2 r1(cosInc, sinInc);
		b2Vec2 v1 = center + radius * r1;
		sf::Color clr = B2ColorConvert(color);
		b2Color tmpFillColor(0.5f * color.r, 0.5f * color.g, 0.5f * color.b, 0.5f);
		sf::Color fillColor = B2ColorConvert(tmpFillColor);
		for (int32 i = 0; i < k_segments; ++i)
		{
			// Perform rotation to avoid additional trigonometry.
			b2Vec2 r2;
			r2.x = cosInc * r1.x - sinInc * r1.y;
			r2.y = sinInc * r1.x + cosInc * r1.y;
			b2Vec2 v2 = center + radius * r2;
			m_triangles.Vertex(sf::Vector2f(v0.x, v0.y), fillColor);
			m_triangles.Vertex(sf::Vector2f(v1.x, v1.y), fillColor);
			m_triangles.Vertex(sf::Vector2f(v2.x, v2.y), fillColor);
			r1 = r2;
			v1 = v2;
		}

		r1.Set(1.0f, 0.0f);
		v1 = center + radius * r1;
		for (int32 i = 0; i < k_segments; ++i)
		{
			b2Vec2 r2;
			r2.x = cosInc * r1.x - sinInc * r1.y;
			r2.y = sinInc * r1.x + cosInc * r1.y;
			b2Vec2 v2 = center + radius * r2;
			m_lines.Vertex(sf::Vector2f(v1.x, v1.y), clr);
			m_lines.Vertex(sf::Vector2f(v2.x, v2.y), clr);
			r1 = r2;
			v1 = v2;
		}

		// Draw a line fixed in the circle to animate rotation.
		b2Vec2 p = center + radius * axis;
		m_lines.Vertex(sf::Vector2f(center.x, center.y), clr);
		m_lines.Vertex(sf::Vector2f(p.x, p.y), clr);
	}

	void DebugDraw::DrawSegment(const b2Vec2& p1, const b2Vec2& p2, const b2Color& color)
	{
		sf::Color clr = B2ColorConvert(color);
		m_lines.Vertex(sf::Vector2f(p1.x, p1.y), clr);
		m_lines.Vertex(sf::Vector2f(p2.x, p2.y), clr);
	}

	float smoothstep(float x) { return x * x * (3 - 2 * x); }
	float currentscale = 1;
	void DebugDraw::DrawParticles(const b2Vec2 *centers, const b2Vec2 *velocities, float32 radius, const b2ParticleColor *colors, int32 count)
	{
		for (int i = 0; i < count; ++i)
		{
			b2Vec2 center = centers[i];
			b2Color color = b2Color(colors[i].r / 255.0f, colors[i].g / 255.0f, colors[i].b / 255.0f);
		
			/*b2Vec2 vel = velocities[i];
			vel.Normalize();
		
			m_lines->Vertex(center - vel, color);
			m_lines->Vertex(center, color);*/
		
			const float32 k_segments = 16.0f;
			const float32 k_increment = 2.0f * b2_pi / k_segments;
			float32 sinInc = sinf(k_increment);
			float32 cosInc = cosf(k_increment);
			b2Vec2 v0 = center;
			b2Vec2 r1(cosInc, sinInc);
			b2Vec2 v1 = center + radius * r1;
			float32 len = velocities[i].Length();
			float32 mod = len;
			sf::Color fillColor(0.5f * color.r * mod, 0.5f * color.g * mod, 0.5f * color.b * mod, 0.5f);
			for (int32 i = 0; i < k_segments; ++i)
			{
				// Perform rotation to avoid additional trigonometry.
				b2Vec2 r2;
				r2.x = cosInc * r1.x - sinInc * r1.y;
				r2.y = sinInc * r1.x + cosInc * r1.y;
				b2Vec2 v2 = center + radius * r2;
				m_triangles.Vertex(sf::Vector2f(v0.x, v0.y), fillColor);
				m_triangles.Vertex(sf::Vector2f(v1.x, v1.y), fillColor);
				m_triangles.Vertex(sf::Vector2f(v2.x, v2.y), fillColor);
				r1 = r2;
				v1 = v2;
			}
		}
	}


	void DebugDraw::DrawTransform(const b2Transform& xf)
	{
		const float32 k_axisScale = 0.4f;
		b2Color red(1.0f, 0.0f, 0.0f);
		b2Color green(0.0f, 1.0f, 0.0f);
		b2Vec2 p1 = xf.p, p2;

		m_lines.Vertex(sf::Vector2f(p1.x, p1.y), sf::Color::Red);
		p2 = p1 + k_axisScale * xf.q.GetXAxis();
		m_lines.Vertex(sf::Vector2f(p2.x, p2.y), sf::Color::Red);

		m_lines.Vertex(sf::Vector2f(p1.x, p1.y), sf::Color::Green);
		p2 = p1 + k_axisScale * xf.q.GetYAxis();
		m_lines.Vertex(sf::Vector2f(p2.x, p2.y), sf::Color::Green);
	}

	void DebugDraw::DrawPoint(const b2Vec2& p, float32 size, const b2Color& color)
	{
		m_points.Vertex(sf::Vector2f(p.x, p.y), sf::Color(color.r, color.g, color.b, color.a));
	}

	void DebugDraw::DrawAABB(b2AABB* aabb, const b2Color& c)
	{
		sf::Color clr(c.r, c.g, c.b, c.a);

		sf::Vector2f p1(aabb->lowerBound.x, aabb->lowerBound.y);
		sf::Vector2f p2(aabb->upperBound.x, aabb->lowerBound.y);
		sf::Vector2f p3(aabb->upperBound.x, aabb->upperBound.y);
		sf::Vector2f p4(aabb->lowerBound.x, aabb->upperBound.y);
		
		m_lines.Vertex(p1, clr);
		m_lines.Vertex(p2, clr);
		
		m_lines.Vertex(p2, clr);
		m_lines.Vertex(p3, clr);
		
		m_lines.Vertex(p3, clr);
		m_lines.Vertex(p4, clr);
		
		m_lines.Vertex(p4, clr);
		m_lines.Vertex(p1, clr);
	}

	void DebugDraw::Flush()
	{
		m_triangles.Flush();
		m_lines.Flush();
		m_points.Flush();
	}

	void DebugDraw::SetRenderWindow(sf::RenderWindow* pRenWin)
	{
		m_points.m_type = sf::PrimitiveType::Points;
		m_lines.m_type = sf::PrimitiveType::Lines;
		m_triangles.m_type = sf::PrimitiveType::Triangles;

		m_points.m_pRenWin = pRenWin;
		m_lines.m_pRenWin = pRenWin;
		m_triangles.m_pRenWin = pRenWin;
	}
}