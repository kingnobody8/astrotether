#include "transform.h"
#include "helper/func.h"

namespace util
{
	STATIC void Transform2D::ValidateTransforms(Transform2D* root)
	{
		Transform2D* curTransform = root;
		while (curTransform != NULL)
		{
			Hierarchy<Transform2D>& curNode = curTransform->m_node;

			// If this transform is dirty, then update the rest of it's hierarchy
			if (curTransform->GetIsDirty())
			{
				// We've got dirt on this transform; we know it's number... Update the transforms on downwards
				curTransform->UpdateHierarchy();
				// Traverse upward to continue our traversal
				while (nullptr != curTransform)
				{
					Hierarchy<Transform2D>& upwardNode = curTransform->m_node;

					// Check if there's a sibling of this node
					if (nullptr != upwardNode.GetSibling())
					{
						curTransform = upwardNode.GetSibling();
						break;
					}
					else
					{
						// If this is the root node, then it's time to leave
						if (root == curTransform)
						{
							curTransform = nullptr;
							break;
						}
						curTransform = upwardNode.GetParent();
					}
				}
			}
			else
			{
				curTransform = curNode.Next();
			}
		}
	}

	Transform2D::Transform2D()
		: m_bDirty(true)
		, m_scale(1.0f)
	{
	}

	Transform2D::Transform2D(const vec2& pos, const vec2& scale, const Angle& rot)
		: m_bDirty(true)
		, m_position(pos)
		, m_scale(scale)
		, m_rotation(rot)
	{
	}

	Transform2D::~Transform2D()
	{
	}

	void Transform2D::Copy(const Transform2D& that)
	{
		*this = that;
	}

	Transform2D& Transform2D::operator=(const Transform2D& that)
	{
		m_position = that.m_position;
		m_scale = that.m_scale;
		m_rotation = that.m_rotation;

		m_dimensions = that.m_dimensions;
		m_dimensionsDelta = that.m_dimensionsDelta;
		m_dimensionsPreserved = that.m_dimensionsPreserved;
		m_aspectPreserved = that.m_aspectPreserved;

		m_pivot = that.m_pivot;
		m_anchor = that.m_anchor;

		m_worldMatrix = that.m_worldMatrix;
		m_worldBounds = that.m_worldBounds;
		m_worldBoundsPreserved = that.m_worldBoundsPreserved;

		m_bDirty = that.m_bDirty;

		return *this;
	}

	const vec2 Transform2D::ParentSpaceToChildSpace(const vec2& parentSpacePosition) const
	{
		vec2 childSpacePosition = parentSpacePosition;

		__todo() //does this work without rotation?
			vec2 unscale = vec2(1.0f, 1.0f);
		vec2 untranslate = vec2(0.0f, 0.0f);
		Transform2D* parent = m_node.GetParent();
		while (parent)
		{
			const vec2& parentScale = parent->GetScale();
			unscale.x *= parentScale.x;
			unscale.y *= parentScale.y;

			const vec2& parentPos = parent->GetPosition();
			untranslate.x += parentPos.x;
			untranslate.y += parentPos.y;

			parent = parent->m_node.GetParent();
		}

		// 'un-apply' the translation
		childSpacePosition -= untranslate;

		// 'un-apply' the scale
		childSpacePosition.x /= unscale.x;
		childSpacePosition.y /= unscale.y;

		return childSpacePosition;
	}

	//Position
	void Transform2D::SetPosition(const vec2& position)
	{
		m_position = position;
		MarkDirty();
	}
	void Transform2D::SetPosition(glm::float32& x, glm::float32& y)
	{
		m_position = vec2(x, y);
		MarkDirty();
	}
	void Transform2D::SetPositionX(const glm::float32& x)
	{
		m_position.x = x;
		MarkDirty();
	}
	void Transform2D::SetPositionY(const glm::float32& y)
	{
		m_position.y = y;
		MarkDirty();
	}

	void Transform2D::Translate(const vec2& position)
	{
		m_position += position;
		MarkDirty();
	}
	void Transform2D::Translate(const glm::float32& x, const glm::float32& y)
	{
		m_position += vec2(x, y);
		MarkDirty();
	}
	void Transform2D::TranslateX(const glm::float32& x)
	{
		m_position.x += x;
		MarkDirty();
	}
	void Transform2D::TranslateY(const glm::float32& y)
	{
		m_position.y += y;
		MarkDirty();
	}

	//Scale
	void Transform2D::SetScale(const vec2& scale)
	{
		m_scale = scale;
		MarkDirty();
	}
	void Transform2D::SetScale(const glm::float32& x, const glm::float32& y)
	{
		m_scale = vec2(x, y);
		MarkDirty();
	}
	void Transform2D::SetScaleX(const glm::float32& x)
	{
		m_scale.x = x;
		MarkDirty();
	}
	void Transform2D::SetScaleY(const glm::float32& y)
	{
		m_scale.y = y;
		MarkDirty();
	}
	void Transform2D::SetScaleUniform(const glm::float32& s)
	{
		m_scale.x = m_scale.y = s;
		MarkDirty();
	}

	void Transform2D::AddScale(const vec2& scale)
	{
		m_scale += scale;
		MarkDirty();
	}
	void Transform2D::AddScale(const glm::float32& x, const glm::float32& y)
	{
		m_scale += vec2(x, y);
		MarkDirty();
	}
	void Transform2D::AddScaleX(const glm::float32& x)
	{
		m_scale.x += x;
		MarkDirty();
	}
	void Transform2D::AddScaleY(const glm::float32& y)
	{
		m_scale.y += y;
		MarkDirty();
	}
	void Transform2D::AddScaleUniform(const glm::float32& s)
	{
		m_scale += vec2(s, s);
		MarkDirty();
	}

	//Rotation
	void Transform2D::SetRotation(const Angle& angle)
	{
		m_rotation = angle;
		MarkDirty();
	}
	void Transform2D::SetRotationDeg(const glm::float32& deg)
	{
		SetRotation(Angle::FromDegrees(deg));
	}
	void Transform2D::SetRotationRad(const glm::float32& rad)
	{
		SetRotation(Angle::FromRadians(rad));
	}
	void Transform2D::AddRotation(const Angle& angle)
	{
		m_rotation += angle;
		MarkDirty();
	}

	//Dimensions
	void Transform2D::SetDimensions(const vec2& dimensions, const bool bUpdateAnchors)
	{
		m_dimensions = dimensions;
		// Update anchors will update the preserved data for us
		if (bUpdateAnchors)
		{
			UpdateAnchors();
		}
		else
		{
			// Update the preserved data, because UpdateAnchors wasn't called
			UpdatePreservedAspectSizes();
		}
		MarkDirty();
	}
	void Transform2D::SetDimensions(const glm::float32& x, const glm::float32& y, const bool bUpdateAnchors)
	{
		SetDimensions(vec2(x, y), bUpdateAnchors);
	}
	void Transform2D::SetDimensionX(const glm::float32& x, const bool bUpdateAnchors)
	{
		SetDimensions(vec2(x, m_dimensions.y), bUpdateAnchors);
	}
	void Transform2D::SetDimensionY(const glm::float32& y, const bool bUpdateAnchors)
	{
		SetDimensions(vec2(m_dimensions.x, y), bUpdateAnchors);
	}

	/*void Transform2D::SetDimensionsDelta(const vec2& deimensionsDelta)
	{
	}
	void Transform2D::SetDimensionsDeltaXY(const glm::float32& dx, const glm::float32& dy)
	{
	}
	void Transform2D::SetDimensionsDeltaX(const glm::float32& dx)
	{
	}
	void Transform2D::SetDimensionsDeltaY(const glm::float32& dy)
	{
	}*/

	//Aspect
	void Transform2D::SetPreservedAspect(const vec2& aspect)
	{
		m_aspectPreserved = aspect;
		UpdatePreservedAspectSizes();
		if (HasPreservedAspect())
			RebuildBounds(m_worldBounds, m_aspectPreserved);
	}
	void Transform2D::SetPreservedAspect(const glm::float32& ax, const glm::float32& ay)
	{
		SetPreservedAspect(vec2(ax, ay));
	}
	void Transform2D::SetPreservedAspectX(const glm::float32& ax)
	{
		SetPreservedAspect(ax, m_aspectPreserved.y);
	}
	void Transform2D::SetPreservedAspectY(const glm::float32& ay)
	{
		SetPreservedAspect(m_aspectPreserved.x, ay);
	}

	//Pivot & Anchor
	void Transform2D::SetAnchor(const AABB& anchor)
	{
		m_anchor = anchor;
	}
	void Transform2D::SetPivot(const vec2& pivot)
	{
		m_pivot = pivot;
		MarkDirty();
	}
	void Transform2D::SetPivot(const float x, const float y)
	{
		SetPivot(vec2(x, y));
	}
	void Transform2D::SetPivotX(const float x)
	{
		SetPivot(x, m_pivot.y);
	}
	void Transform2D::SetPivotY(const float y)
	{
		SetPivot(m_pivot.x, y);
	}


	const Matrix& Transform2D::GetWorldMatrix()
	{
		ValidateCleanliness();
		return m_worldMatrix;
	}
	const Box& Transform2D::GetWorldBounds()
	{
		ValidateCleanliness();
		return m_worldBounds;
	}
	const Matrix& Transform2D::GetWorldTransform() const
	{
		return m_worldMatrix;
	}
	const Box& Transform2D::GetWorldBounds() const
	{
		return m_worldBounds;
	}

	void Transform2D::RebuildTransform(Matrix& outMat)
	{
		vec2 sizePivot(m_pivot);
		// since pivots are from 0->1, we need to translate them to be from -0.5->0.5
		sizePivot *= 2;
		sizePivot -= vec2(1.0f, 1.0f);
		sizePivot *= -0.5f;
		sizePivot.x *= m_dimensions.x;
		sizePivot.y *= m_dimensions.y;

		outMat.BuildTransformationMatrix(m_position, m_scale, m_rotation, sizePivot);
	}
	void Transform2D::RebuildBounds(Box& bounds, const vec2& dimensionsPreserved)
	{
		const vec2 translation = m_worldMatrix.GetTranslation();
		const vec2 scale = m_worldMatrix.CalculateScale() * 0.5f;
		bounds.lowerBound.Set(translation.x - (scale.x * dimensionsPreserved.x), translation.y - (scale.y * dimensionsPreserved.y));
		bounds.upperBound.Set(translation.x + (scale.x * dimensionsPreserved.x), translation.y + (scale.y * dimensionsPreserved.y));
		bounds.SetRotation(m_worldMatrix.CalculateRotation());
	}
	void Transform2D::UpdatePreservedAspectSizes()
	{
		if (!HasPreservedAspect())
			return;

		m_dimensionsPreserved = m_dimensions;
		const glm::float32 origAspect = m_aspectPreserved.x / m_aspectPreserved.y;
		const glm::float32 inAspect = m_dimensions.x / m_dimensions.y;
		const glm::float32 higherAspect = Max(origAspect, inAspect);
		if (higherAspect == inAspect)
			m_dimensionsPreserved.x = m_dimensions.y * origAspect;
		else
			m_dimensionsPreserved.y = m_dimensions.x / origAspect;
	}
	void Transform2D::UpdateAnchors()
	{
		Box oldBound(m_worldBounds);
		RebuildBounds(m_worldBounds, m_dimensions);

		// Update the preserved data
		UpdatePreservedAspectSizes();

		Transform2D* childTransform = m_node.GetChild();
		while (childTransform)
		{
			childTransform->ApplyAnchors(oldBound);
			childTransform->UpdateAnchors();
			childTransform = childTransform->m_node.GetSibling();
		}
	}
	void Transform2D::ApplyAnchors(const Box& prevBounds)
	{
		const Transform2D* parent = m_node.GetParent();
		const Transform2D* constThis = this;
		if (nullptr == parent)
			return;

		const Box& parentBounds = parent->GetWorldBounds();
		Box newBounds = constThis->GetWorldBounds();

		// min.x
		{
			// 
			const float anchorInf = 1.0f - (m_anchor.lowerBound.x * 2.0f);
			const float diff = parentBounds.lowerBound.x - prevBounds.lowerBound.x;
			const float endDistance = diff * anchorInf;
			m_dimensions.x += -endDistance;
			newBounds.lowerBound.x += endDistance;
		}

		// max.x
		{
			const float anchorInf = 1.0f - ((1.0f - m_anchor.upperBound.x) * 2.0f);
			const float diff = parentBounds.upperBound.x - prevBounds.upperBound.x;
			const float endDistance = diff * anchorInf;
			m_dimensions.x += endDistance;
			newBounds.upperBound.x += endDistance;
		}

		// min.y
		{
			const float anchorInf = 1.0f - (m_anchor.lowerBound.y * 2.0f);
			const float diff = parentBounds.lowerBound.y - prevBounds.lowerBound.y;
			const float endDistance = diff * anchorInf;
			m_dimensions.y += -endDistance;
			newBounds.lowerBound.y += endDistance;
		}

		// max.y
		{
			const float anchorInf = 1.0f - ((1.0f - m_anchor.upperBound.y) * 2.0f);
			const float diff = parentBounds.upperBound.y - prevBounds.upperBound.y;
			const float endDistance = diff * anchorInf;
			m_dimensions.y += endDistance;
			newBounds.upperBound.y += endDistance;
		}

		m_position = (newBounds.CalculatePointWithPivot(m_pivot) - parentBounds.CalculateCenter()) / parent->GetWorldTransform().CalculateScale();
	}
	void Transform2D::UpdateHierarchy()
	{
		m_bDirty = false;
		Matrix localMatrix;
		// Rebuild your local Matrix
		RebuildTransform(localMatrix);
		
		// Factor in the parent transform
		Transform2D* parent = m_node.GetParent();
		if (parent)
		{
			m_worldMatrix = parent->GetWorldTransform();
			m_worldMatrix.SelfMultiply(localMatrix);
		}
		else
		{
			m_worldMatrix = localMatrix;
		}
		
		// Rebuild the bounds
		RebuildBounds(m_worldBounds, m_dimensions);
		
		// Rebuild the preserved bounds, if there are any
		if (HasPreservedAspect())
			RebuildBounds(m_worldBoundsPreserved, m_dimensionsPreserved);
		
		// Inform your children of these updates
		Transform2D* childTransform = m_node.GetChild();
		while (childTransform)
		{
			childTransform->UpdateHierarchy();
			childTransform = childTransform->m_node.GetSibling();
		}
	}
	void Transform2D::ValidateCleanliness()
	{
		Transform2D* lastDirty = nullptr;
		Transform2D* curNode = this;
		while (curNode)
		{
			if (curNode->GetIsDirty())
				lastDirty = curNode;
			curNode = curNode->m_node.GetParent();
		}
		if (lastDirty)
		{
			ValidateTransforms(lastDirty);
		}
	}
}