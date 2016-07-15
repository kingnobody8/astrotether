#include "camera.h"
#include "utility/helper/func.h"
#include "render_plugin.h"

namespace engine
{
	namespace render
	{
		STATIC std::list<CCamera*> CCamera::s_cameras;

		STATIC CCamera* CCamera::FindCamera(const std::string& name)
		{
			for (auto iter = s_cameras.begin(); iter != s_cameras.end(); ++iter)
			{
				if ((*iter)->GetName() == name)
				{
					return (*iter);
				}
			}
			return null;
		}
		STATIC CCamera* CCamera::CreateCamera(const std::string& name)
		{
			assert(!name.empty());

			CCamera* cam = new CCamera(name);
			s_cameras.push_back(cam);
			return cam;
		}
		STATIC void CCamera::DestroyCameras()
		{
			for (auto iter = s_cameras.begin(); iter != s_cameras.end(); ++iter)
				delete (*iter);
			s_cameras.clear();
		}
		
		CCamera::CCamera(const std::string& name)
			: m_name(name)
		{
		}

		CCamera::~CCamera()
		{
		}

		const b2AABB CCamera::CalcViewAabb() const
		{
			__todo();
			//SDL_GetWindowSize()
			const vec2 half_screen_dims;

			//create an aabb with the screen dims
			b2AABB ret;
			ret.lowerBound = b2Vec2(-half_screen_dims.x, -half_screen_dims.y);
			ret.upperBound = b2Vec2(half_screen_dims.y, half_screen_dims.y);
		
			//modify the aabb by our inverse Matrix

			__todo() // check this Matrix math

				util::Matrix inverse;// = glm::inverse(m_matrix);
			vec4 min;// = inverse * vec4(ret.lowerBound.x, ret.lowerBound.y, 0.0f, 1.0f);
			vec4 max;// = inverse * vec4(ret.upperBound.x, ret.upperBound.y, 0.0f, 1.0f);
			//	util::Matrix2D::Vector_Matrix_Multiply(util::vec2(ret.lowerBound.x, ret.lowerBound.y), inverse);
			//vec2 max = util::Matrix2D::Vector_Matrix_Multiply(util::vec2(ret.upperBound.x, ret.upperBound.y), inverse);

			ret.lowerBound = b2Vec2(Min(min.x, max.x), Min(min.y, max.y));
			ret.upperBound = b2Vec2(Max(min.x, max.x), Max(min.y, max.y));

			return ret;
		}
	}
}