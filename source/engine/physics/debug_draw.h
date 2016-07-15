#pragma once
#include "Box2D/Box2D.h"
#include "../render/render_core.h"

namespace engine
{
	namespace physics
	{
		struct GLRenderPoints;
		struct GLRenderLines;
		struct GLRenderTriangles
		{
			void Create();
			void Destroy();
			void Vertex(const b2Vec2& v, const b2Color& c);
			void Flush();

			enum { e_maxVertices = 3 * 512 };
			b2Vec2 m_vertices[e_maxVertices];
			b2Color m_colors[e_maxVertices];

			int32 m_count;

			//			GLuint m_vaoId;
			GLuint m_vboIds[2];
			GLuint m_programId;
			GLint m_projectionUniform;
			GLint m_vertexAttribute;
			GLint m_colorAttribute;
			bool useCamera;
		};

		struct Camera
		{
			Camera()
			{
				m_center.Set(0.0f, 20.0f);
				m_extent = 25.0f;
				m_zoom = 1.0f;
				m_width = 1096;//1280;
				m_height = 640;//720;
			}

			b2Vec2 ConvertScreenToWorld(const b2Vec2& screenPoint);
			b2Vec2 ConvertWorldToScreen(const b2Vec2& worldPoint);
			void BuildProjectionMatrix(float32* m, float32 zBias);

			b2Vec2 m_center;
			float32 m_extent;
			float32 m_zoom;
			int32 m_width;
			int32 m_height;
		};

		// This class implements debug drawing callbacks that are invoked inside b2World::Step.
		class DebugDraw : public b2Draw
		{
		public:
			DebugDraw();
			~DebugDraw();

			void Create();
			void Destroy();

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

		private:
			GLRenderPoints* m_points;
			GLRenderLines* m_lines;
			GLRenderTriangles* m_triangles;
		};

		extern Camera g_camera;
	}
}


