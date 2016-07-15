#pragma once
#include "util_math.h"
#include "angle.h"
#include "Matrix.h"
#include "helper/hierarchy.h"
#include "box.h"

namespace util
{
	class Transform2D
	{
	public:
		static void ValidateTransforms(Transform2D* root);

		Transform2D();
		Transform2D(const vec2& pos, const vec2& scale, const Angle& rot);
		~Transform2D();

		void Copy(const Transform2D& that);
		Transform2D& operator=(const Transform2D& that);

		inline const vec2 GetPosition() const { return m_position; }
		inline const Angle& GetRotation() const { return m_rotation; }
		inline const vec2 GetScale() const { return m_scale; }

		inline const vec2& GetDimensions() const { return m_dimensions; }
		inline const vec2& GetDimensionsPreserved() const { return m_dimensionsPreserved; }
		inline const vec2& GetDimensionsDelta() const { return m_dimensionsDelta; }

		inline const AABB GetAnchor() const { return m_anchor; }
		inline const vec2& GetPivot() const { return m_pivot; }

		inline bool HasPreservedAspect() const { return !(m_aspectPreserved.x == 0.0f || m_aspectPreserved.y == 0.0f); }
		inline const vec2& GetAspectPreserved() const { return m_aspectPreserved; }

		const vec2 ParentSpaceToChildSpace(const vec2& parentSpacePosition) const;

		//Position
		void SetPosition(const vec2& position);
		void SetPosition(glm::float32& x, glm::float32& y);
		void SetPositionX(const glm::float32& x);
		void SetPositionY(const glm::float32& y);

		void Translate(const vec2& position);
		void Translate(const glm::float32& x, const glm::float32& y);
		void TranslateX(const glm::float32& x);
		void TranslateY(const glm::float32& y);

		//Scale
		void SetScale(const vec2& scale);
		void SetScale(const glm::float32& x, const glm::float32& y);
		void SetScaleX(const glm::float32& x);
		void SetScaleY(const glm::float32& y);
		void SetScaleUniform(const glm::float32& s);

		void AddScale(const vec2& scale);
		void AddScale(const glm::float32& x, const glm::float32& y);
		void AddScaleX(const glm::float32& x);
		void AddScaleY(const glm::float32& y);
		void AddScaleUniform(const glm::float32& s);

		//Rotation
		void SetRotation(const Angle& angle);
		void SetRotationDeg(const glm::float32& deg);
		void SetRotationRad(const glm::float32& rad);
		void AddRotation(const Angle& angle);
		
		//Dimensions
		void SetDimensions(const vec2& dimensions, const bool bUpdateAnchors);
		void SetDimensions(const glm::float32& x, const glm::float32& y, const bool bUpdateAnchors);
		void SetDimensionX(const glm::float32& x, const bool bUpdateAnchors);
		void SetDimensionY(const glm::float32& y, const bool bUpdateAnchors);

		/*void SetDimensionsDelta(const vec2& deimensionsDelta);
		void SetDimensionsDeltaXY(const glm::float32& dx, const glm::float32& dy);
		void SetDimensionsDeltaX(const glm::float32& dx);
		void SetDimensionsDeltaY(const glm::float32& dy);*/
		
		void SetPreservedAspect(const vec2& aspect);
		void SetPreservedAspect(const glm::float32& ax, const glm::float32& ay);
		void SetPreservedAspectX(const glm::float32& ax);
		void SetPreservedAspectY(const glm::float32& ay);

		void SetAnchor(const AABB& anchor);
		void SetPivot(const vec2& pivot);
		void SetPivot(const float x, const float y);
		void SetPivotX(const float x);
		void SetPivotY(const float y);

		inline const bool GetIsDirty() const { return m_bDirty; }

		const Matrix& GetWorldMatrix();
		const Box& GetWorldBounds();

	protected:
		const Matrix& GetWorldTransform() const;
		const Box& GetWorldBounds() const;
		inline void MarkDirty() { m_bDirty = true; }

	private:
		void RebuildTransform(Matrix& outMat);
		void RebuildBounds(Box& bounds, const vec2& dimensionsPreserved);
		void UpdatePreservedAspectSizes();
		void UpdateAnchors();
		void ApplyAnchors(const Box& prevBounds);
		void UpdateHierarchy();
		void ValidateCleanliness();

	protected:
		Hierarchy<Transform2D> m_node;

	private:
		vec2 m_position;
		vec2 m_scale;
		Angle m_rotation;

		vec2 m_dimensions;
		vec2 m_dimensionsPreserved;
		vec2 m_dimensionsDelta;
		vec2 m_aspectPreserved;

		AABB m_anchor;
		vec2 m_pivot;

		Matrix m_worldMatrix;
		Box m_worldBounds;
		Box m_worldBoundsPreserved;

		bool m_bDirty;

	};
}
