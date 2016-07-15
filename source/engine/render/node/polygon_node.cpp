#include "polygon_node.h"
#include "utility/helper/func.h"
#include "../shader/shader_default.h"

namespace engine
{
	namespace render
	{
		PolygonNode::PolygonNode()
			: m_vVerts(null)
			, m_vertCount(0)
			, m_vIndicies(null)
			, m_indexCount(0)
		{
		}

		VIRTUAL PolygonNode::~PolygonNode()
		{
			FreeVBO();
		}

		void PolygonNode::InitVBO(const std::vector<Vertex>& verts, const std::vector<int>& indicies)
		{
			//early out
			if (verts.empty())
				return;

			//Cleanup any old data
			FreeVBO();

			m_vertCount = verts.size();
			m_vVerts = new Vertex[m_vertCount];
			for (int i = 0; i < m_vertCount; ++i)
			{
				m_vVerts[i] = verts[i];
			}

			m_indexCount = indicies.size();
			m_vIndicies = new int[m_indexCount];
			for (int i = 0; i < m_indexCount; i++)
			{
				m_vIndicies[i] = indicies[i];
			}

			//Create VBO
			glGenBuffers(1, &m_vboID);
			glBindBuffer(GL_ARRAY_BUFFER, m_vboID);
			glBufferData(GL_ARRAY_BUFFER, m_vertCount * sizeof(Vertex), m_vVerts, GL_DYNAMIC_DRAW);

			//Create IBO
			glGenBuffers(1, &m_iboID);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_iboID);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indexCount * sizeof(GLuint), m_vIndicies, GL_DYNAMIC_DRAW);

			//Unbind buffers
			glBindBuffer(GL_ARRAY_BUFFER, NULL);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, NULL);
		}

		VIRTUAL void PolygonNode::FreeVBO()
		{
			if (m_vboID != 0)
			{
				glDeleteBuffers(1, &m_vboID);
				glDeleteBuffers(1, &m_iboID);

				SafeDelete(m_vVerts);
				m_vertCount = 0;
				SafeDelete(m_vIndicies);
				m_indexCount = 0;
			}
		}

		VIRTUAL void PolygonNode::CalcAabbInternal()
		{
			m_aabb = b2AABB();
			for (int i = 0; i < m_vertCount; ++i)
			{
				vec4 tmp = m_matrix * vec4(m_vVerts[i].position.x, m_vVerts[i].position.y, 0.0f, 1.0f);
				if (!m_aabb.IsValid())
				{
					m_aabb.lowerBound = m_aabb.upperBound = b2Vec2(tmp.x, tmp.y);
				}
				else
				{
					m_aabb.Combine(b2Vec2(tmp.x, tmp.y));
				}
			}
		}

		VIRTUAL void PolygonNode::operator() (const util::Matrix& inv_cam)
		{
			if (m_vboID == 0 || m_pShader == null)
				return;

			m_pShader->Bind();

			glEnable(GL_BLEND);
			glDisable(GL_DEPTH_TEST);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

			DefaultShader* dShader = static_cast<DefaultShader*>(m_pShader);
			dShader->EnableVertexPos2D();
			dShader->EnableVertexColor();
			
			GLenum err = glGetError();

			glBindBuffer(GL_ARRAY_BUFFER, m_vboID);
			dShader->SetVertexPos2D(sizeof(Vertex), (GLvoid*)offsetof(Vertex, position));
			dShader->SetVertexColor(sizeof(Vertex), (GLvoid*)offsetof(Vertex, color));
			
			err = glGetError();
			
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_iboID);
			glDrawElements(GL_TRIANGLE_FAN, m_indexCount, GL_UNSIGNED_INT, NULL);

			err = glGetError();
			
			glBindBuffer(GL_ARRAY_BUFFER, NULL);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, NULL);
			
			dShader->DisableVertexPos2D();
			dShader->DisableVertexColor();

			m_pShader->Unbind();

		}

		const std::vector<Vertex> PolygonNode::GetVerts()
		{
			std::vector<Vertex> ret;
			ret.resize(m_vertCount);
			for (int i = 0; i < m_vertCount; ++i)
			{
				ret[i] = m_vVerts[i];
			}
			return ret;
		}

		const std::vector<int> PolygonNode::GetIndicies()
		{
			std::vector<int> ret;
			ret.resize(m_indexCount);
			for (int i = 0; i < m_indexCount; ++i)
			{
				ret[i] = m_vIndicies[i];
			}
			return ret;
		}
	}
}