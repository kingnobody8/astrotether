#include "component/object.h"
#include "component/part.h"
#include <stack>
#include <queue>

namespace baka
{
	namespace component
	{
		STATIC std::vector<CObject*> CObject::s_MarkedForDestruction;

		STATIC void CObject::Nuke(CObject* that)
		{
			that->Nuke();
		}

		STATIC void CObject::Clean()
		{
			for (size_t i = 0; i < s_MarkedForDestruction.size(); ++i)
			{
				s_MarkedForDestruction[i]->Nuke();
			}
			s_MarkedForDestruction.clear();
		}


		/*------------------------------------------------------------------*/
		/*								COBJECT								*/
		/*------------------------------------------------------------------*/
		CObject::CObject(void)
			: m_pParent(null)
			, m_fZed(0.0f)
			, m_eFlag(EFlag::ACTIVE | EFlag::SELF_VISIBLE | EFlag::PARENT_VISIBLE)
		{
		}
		VIRTUAL CObject::~CObject(void)
		{
		}
		CObject::CObject(const CObject& that)
		{
			*this = that;
		}
		CObject& CObject::operator=	(const CObject& that)
		{
			//Check for self assignment
			if (this == &that)
				return *this;

			assert(this->IsGroup() == that.IsGroup() && "Can't copy groups to objects and vice versa");

			//Uninitialize all current data
			this->Exit();

			//Fill out members
			this->m_szName = that.m_szName;
			this->m_pParent = null;
			this->m_cMatLocal = that.m_cMatLocal;
			this->m_fZed = that.m_fZed;
			this->m_eFlag = that.m_eFlag;

			const size_t size = that.m_vParts.size();
			this->m_vParts.reserve(size);
			for (size_t i = 0; i < size; ++i)
			{
				this->AttachPart(that.m_vParts[i]->Clone());
			}
			return *this;
		}

		//Interface
		VIRTUAL void CObject::Init(void)
		{
			std::list<IPart*> list = GetPartList(this);
			for (auto iter = list.begin(); iter != list.end(); ++iter)
			{
				(*iter)->Init();
			}
			m_eFlag.FlagOn(STARTED);
		}
		VIRTUAL void CObject::Exit(void)
		{
			std::list<IPart*> list = GetPartList(this, true);
			for (auto iter = list.begin(); iter != list.end(); ++iter)
			{
				(*iter)->Exit();
			}
			m_eFlag.FlagOn(STOPPED);
		}
		VIRTUAL void CObject::Reset(void)
		{
			this->Exit();
			this->Init();
		}

		VIRTUAL void CObject::Copy(const CObject& that)
		{
			*this = that;
		}
		VIRTUAL void CObject::Copy(const CGroup& that)
		{
			assert(false && "Shouldn't ever get here, objects can't copy groups");
		}
		VIRTUAL CObject* CObject::Clone(void)
		{
			CObject* ret = new CObject();
			ret->Copy(*this);
			return ret;
		}

		VIRTUAL void CObject::Drop(void)
		{
			assert(m_pParent);
			this->m_pParent->RemoveObject(this);
		}
		void CObject::Destroy(void)
		{
			assert(!m_eFlag.Flag(EFlag::NUKED));

			if (this->m_pParent)
				this->m_pParent->RemoveObject(this);

			m_eFlag.FlagOn(EFlag::NUKED);
			s_MarkedForDestruction.push_back(this);
		}
		VIRTUAL void CObject::Nuke(void)
		{
			assert(m_eFlag.Flag(EFlag::NUKED));

			std::list<IPart*> list = GetPartList(this, true);
			for (auto iter = list.begin(); iter != list.end(); ++iter)
			{
				(*iter)->Destroy();
			}

			delete this;
		}


		VIRTUAL void CObject::AttachPart(IPart* const pPart)
		{
			assert(pPart);
			pPart->SetOwner(this);
			this->m_vParts.push_back(pPart);
			pPart->OnAttach();
		}
		VIRTUAL bool CObject::DetachPart(IPart* const pPart)
		{
			assert(pPart);
			const size_t size = this->m_vParts.size();
			for (size_t i = 0; i < size; ++i)
			{
				if (this->m_vParts[i] == pPart)
				{
					this->m_vParts[i]->Destroy();
					this->m_vParts.erase(this->m_vParts.begin() + i);
					return true;
				}
			}
			return false;
		}

		VIRTUAL void CObject::LoadJson(const util::JSON& json)
		{
			//Object
			this->m_szName = json["name"].GetString();
			bool visible = json["visible"].GetBool();
			SetVisible(visible);
			this->m_fZed = json["Matrix"]["position"]["z"].GetDouble();
			this->SetLocalMatrix(((const util::JSON)(json["Matrix"])).GetMatrix());
			const std::vector<util::JSON> parts = ((const util::JSON)(json["parts"])).GetArray<util::JSON>();
			//Parts
			for (ulong i = 0; i < parts.size(); ++i)
			{
				std::string type = parts[i]["type"].GetString();
				IPart* pPart = IPart::CreatePart(type);
				pPart->LoadJson(parts[i]);
				this->AttachPart(pPart);
			}
		}
		VIRTUAL const util::JSON CObject::SaveJson(void) const
		{
			__not_implemented(CObject::SaveJson);
			return util::JSON();
		}

		//Calc
		const util::Matrix CObject::CalcWorldMatrix(void) const
		{
			util::Matrix ret = m_cMatLocal;
			TraverseAncestors(this, [&ret](CObject* pObj)
			{
				ret = ret * pObj->GetLocalMatrix();
			});

			return ret;
		}
		const float CObject::CalcWorldZed(void) const
		{
			float ret = this->m_fZed;
			TraverseAncestors(this, [&ret](CObject* pObj)
			{
				ret += pObj->GetLocalZed();
			});
			return ret;
		}
		const b2AABB CObject::CalcAabb(void) const
		{
			b2AABB aabb;
			aabb.lowerBound = b2Vec2(1.0f, 1.0f);
			aabb.upperBound = b2Vec2(-1.0f, -1.0f);
			std::list<IPart*> list = GetPartList(this);
			for (auto iter = list.begin(); iter != list.end(); ++iter)
			{
				const b2AABB tmp = (*iter)->CalcAABB();
				if (!aabb.IsValid() && tmp.IsValid())
					aabb = tmp;
				else if (tmp.IsValid())
					aabb.Combine(tmp);
			}
			return aabb;
		}

		//Sets
		void CObject::SetParent(CGroup* const pParent)
		{
			if (this->m_pParent)
				this->Drop();
			this->m_pParent = pParent;
		}
		void CObject::SetLocalMatrix(const util::Matrix& mat)
		{
			m_cMatLocal = mat;
			OnMatrixChanged();
		}
		void CObject::SetWorldMatrix(const util::Matrix& mat)
		{
		/*	util::Matrix temp = mat;
			if (this->m_pParent)
			{
				temp = this->m_pParent->CalcWorldMatrix() * glm::inverse(mat.);
			}
			this->SetLocalMatrix(temp);*/
		}

		void CObject::SetLocalPosX(const float& x)
		{
			//this->m_cMatLocal.SetPositionX(x);
			OnMatrixChanged();
		}
		void CObject::SetLocalPosY(const float& y)
		{
			//this->m_cMatLocal.SetPositionY(y);
			OnMatrixChanged();
		}
		void CObject::SetLocalPosXY(const float& x, const float& y)
		{
			//this->m_cMatLocal.SetPosition(x, y, this->m_cMatLocal.GetPosition().z);
			OnMatrixChanged();
		}
		void CObject::SetLocalScale(const float& x, const float& y)
		{
			//this->m_cMatLocal.SetScale(x, y, this->m_cMatLocal.GetScale().z);
			OnMatrixChanged();
		}
		void CObject::SetLocalRotationZ(const float& rot)
		{
			//util::vec3 rotation = this->m_cMatLocal.GetRotation();
			//rotation.z = rot;
			//this->m_cMatLocal.SetRotation(rotation);
			OnMatrixChanged();
		}

		void CObject::SetWorldPosX(const float& x)
		{
			/*if (m_pParent)
			{
				const Matrix& wmat = this->m_pParent->CalcWorldMatrix();
				vec3 wpos = this->m_pParent->CalcWorldMatrix().GetPosition();
				float pos_x = x / wmat.GetScale().x - wpos.x;
				this->SetLocalPosX(pos_x);
			}
			else
				this->SetLocalPosX(x);*/
		}
		void CObject::SetWorldPosY(const float& y)
		{
			/*if (m_pParent)
			{
				const Matrix& wmat = this->m_pParent->CalcWorldMatrix();
				vec3 wpos = this->m_pParent->CalcWorldMatrix().GetPosition();
				float pos_y = y / wmat.GetScale().y - wpos.y;
				this->SetLocalPosY(pos_y);
			}
			else
				this->SetLocalPosY(y);*/
		}
		void CObject::SetWorldPosXY(const float& x, const float& y)
		{
			__todo() //fix this
			/*if (m_pParent)
			{
				const Matrix& wmat = this->m_pParent->CalcWorldMatrix();
				vec3 wpos = this->m_pParent->CalcWorldMatrix().GetPosition();
				vec2 pos = vec2(x, y, 1.0f) / wmat.GetScale() - wpos;
				this->SetLocalPosXY(pos);
			}
			else
			{
				this->SetLocalPosXY(x, y);
			}*/
		}
		void CObject::SetWorldScale(const float& x, const float& y)
		{
			/*if (m_pParent)
			{
				vec3 wscale = this->m_pParent->CalcWorldMatrix().GetScale();
				this->SetLocalScale(x - wscale.x, y - wscale.y);
			}
			else
			{
				this->SetLocalScale(x, y);
			}*/
		}
		void CObject::SetWorldRotationZ(const float& rot)
		{
			/*if (m_pParent)
				this->SetLocalRotationZ(rot - this->m_pParent->CalcWorldMatrix().GetRotation().z);
			else
				this->SetLocalRotationZ(rot);*/
		}

		VIRTUAL void CObject::SetVisible(const bool bVis)
		{
			m_eFlag.FlagBool(EFlag::SELF_VISIBLE, bVis);
			OnVisibilityChanged(bVis);
		}

		//Find
		IPart* CObject::FindPart(const std::string& szType) const
		{
			const size_t size = this->m_vParts.size();
			for (size_t i = 0; i < size; ++i)
			{
				if (this->m_vParts[i]->GetTypeName() == szType)
				{
					return this->m_vParts[i];
				}
			}
			return null;
		}
		IPart* CObject::FindPart(const PartTypeKey& key) const
		{
			const size_t size = this->m_vParts.size();
			for (size_t i = 0; i < size; ++i)
			{
				if (this->m_vParts[i]->GetType() == key)
				{
					return this->m_vParts[i];
				}
			}
			return null;
		}

		//Protected
		void CObject::OnMatrixChanged(void)
		{
			std::list<IPart*> list = GetPartList(this);
			for (auto iter = list.begin(); iter != list.end(); ++iter)
			{
				(*iter)->OnMatrixChanged();
			}

			list = GetAncestorPartList(this);
			for (auto iter = list.begin(); iter != list.end(); ++iter)
			{
				(*iter)->OnChildMatrixChanged(this);
			}
		}
		void CObject::OnZedChanged(void)
		{
			std::list<IPart*> list = GetPartList(this);
			for (auto iter = list.begin(); iter != list.end(); ++iter)
			{
				(*iter)->OnZedChanged();
			}

			list = GetAncestorPartList(this);
			for (auto iter = list.begin(); iter != list.end(); ++iter)
			{
				(*iter)->OnChildZedChanged(this);
			}
		}
		void CObject::OnVisibilityChanged(const bool bVisible)
		{
			std::list<IPart*> list = GetPartList(this);
			for (auto iter = list.begin(); iter != list.end(); ++iter)
			{
				(*iter)->OnVisibilityChanged(GetWorldVisible());
			}

			list = GetAncestorPartList(this);
			for (auto iter = list.begin(); iter != list.end(); ++iter)
			{
				(*iter)->OnChildVisibilityChanged(this);
			}
		}
		void CObject::OnParentVisibilityChanged(const bool bVisible)
		{
			this->m_eFlag.FlagBool(EFlag::PARENT_VISIBLE, bVisible);
			std::list<IPart*> list = GetPartList(this);
			for (auto iter = list.begin(); iter != list.end(); ++iter)
			{
				(*iter)->OnVisibilityChanged(GetWorldVisible());
			}
		}

		/*------------------------------------------------------------------*/
		/*								CGROUP								*/
		/*------------------------------------------------------------------*/
		CGroup::CGroup(void)
		{
		}
		VIRTUAL CGroup::~CGroup(void)
		{
		}
		CGroup::CGroup(const CGroup& that)
		{
			*this = that;
		}
		CGroup& CGroup::operator = (const CGroup& that)
		{
			CObject::operator=(that);

			const size_t size = that.m_vChildren.size();
			this->m_vChildren.reserve(size);
			for (size_t i = 0; i < size; ++i)
			{
				this->AppendObject(that.m_vChildren[i]->Clone());
			}
			return *this;
		}

		//Interface
		VIRTUAL void CGroup::Copy(const CObject& that)
		{
			assert(false && "Shouldn't ever get here, groups can't copy objects");
		}
		VIRTUAL void CGroup::Copy(const CGroup& that)
		{
			*this = that;
		}
		VIRTUAL CObject* CGroup::Clone(void)
		{
			CGroup* ret = new CGroup();
			ret->Copy(*this);
			return ret;
		}

		VIRTUAL void CGroup::AppendObject(CObject* const pObj)
		{
			assert(pObj);
			this->m_vChildren.push_back(pObj);
			pObj->SetParent(this);

			std::list<IPart*> list = GetAncestorPartList(pObj);
			for (auto iter = list.begin(); iter != list.end(); ++iter)
			{
				(*iter)->OnChildAppended(pObj);
			}
		}
		VIRTUAL bool CGroup::RemoveObject(CObject* const pObj)
		{
			assert(pObj);
			const size_t size = this->m_vChildren.size();
			for (size_t i = 0; i < size; ++i)
			{
				if (this->m_vChildren[i] == pObj)
				{
					this->m_vChildren.erase(this->m_vChildren.begin() + i);

					std::list<IPart*> list = GetAncestorPartList(pObj);
					for (auto iter = list.begin(); iter != list.end(); ++iter)
					{
						(*iter)->OnChildAppended(pObj);
					}

					return true;
				}
			}
			return false;
		}

		VIRTUAL void CGroup::Nuke(void)
		{
			assert(m_eFlag.Flag(EFlag::NUKED));

			std::list<IPart*> list = GetPartList(this, true);
			for (auto iter = list.begin(); iter != list.end(); ++iter)
			{
				(*iter)->Destroy();
			}

			std::list<CObject*> children;
			TraverseChildren(this, [&children](CObject* pChild)
			{
				children.push_back(pChild);
			});
			for (auto iter = children.begin(); iter != children.end(); ++iter)
			{
				delete (*iter);
			}

			delete this;
		}

		VIRTUAL void CGroup::LoadJson(const util::JSON& json)
		{
			//Object
			CObject::LoadJson(json);
			//Children
			std::vector<util::JSON> children = ((const util::JSON)(json["children"])).GetArray<util::JSON>();
			for (ulong i = 0; i < children.size(); ++i)
			{
				CObject* pObj = LoadObjectFromJson(children[i]);
				this->AppendObject(pObj);
			}
		}
		VIRTUAL const util::JSON CGroup::SaveJson(void) const
		{
			__not_implemented(CObject::SaveJson);
			return util::JSON();
		}

		//Sets
		VIRTUAL void CGroup::SetVisible(const bool bVis)
		{
			m_eFlag.FlagBool(EFlag::SELF_VISIBLE, bVis);
			TraverseChildren(this, [&bVis](CObject* pChild)
			{
				pChild->OnParentVisibilityChanged(bVis);
			});
			OnVisibilityChanged(bVis);
		}

		//Finds
		CObject* CGroup::FindObject(const std::string& szName
			, const int& depth) const
		{
			__todo()// once we get a depth first traversal func we should use that here

			const size_t size = this->m_vChildren.size();
			for (size_t i = 0; i < size; ++i)
			{
				if (this->m_vChildren[i]->GetName() == szName)
				{
					return this->m_vChildren[i];
				}
			}

			//If we get here then search our children's children for the object
			for (size_t i = 0; i < size; ++i)
			{
				if (this->m_vChildren[i]->IsGroup())
				{
					CGroup* pGroup = static_cast<CGroup*>(this->m_vChildren[i]);
					CObject* ret = pGroup->FindObject(szName);
					if (ret)
						return ret;
				}
			}
			return null;
		}

		CGroup* CGroup::FindGroup(const std::string& szName
			, const int& depth) const
		{
			__todo()// once we get a depth first traversal func we should use that here

			const size_t size = this->m_vChildren.size();
			for (size_t i = 0; i < size; ++i)
			{
				if (this->m_vChildren[i]->GetName() == szName && this->m_vChildren[i]->IsGroup())
				{
					return static_cast<CGroup*>(this->m_vChildren[i]);
				}
			}

			//If we get here then search our children's children for the object
			for (size_t i = 0; i < size; ++i)
			{
				if (this->m_vChildren[i]->IsGroup())
				{
					CGroup* pGroup = static_cast<CGroup*>(this->m_vChildren[i]);
					CGroup* ret = pGroup->FindGroup(szName);
					if (ret)
						return ret;
				}
			}
			return null;
		}

		/*------------------------------------------------------------------*/
		/*								FUNCTIONS							*/
		/*------------------------------------------------------------------*/
		void Update(CObject* const pObj, const util::Time& dt)
		{
			assert(pObj);
			std::list<IPart*> list = GetPartList(pObj);
			for (auto iter = list.begin(); iter != list.end(); ++iter)
			{
				(*iter)->Update(dt);
			}
		}
		
		const bool IsRenderable(const CObject* const pObj)
		{
			assert(pObj);
			std::list<IPart*> list = GetPartList(pObj);
			for (auto iter = list.begin(); iter != list.end(); ++iter)
			{
				if ((*iter)->IsRenderable())
					return true;
			}
			return false;
		}

		CObject* LoadObjectFromJson(const util::JSON& json)
		{
			auto asdf = json.Read();

			CObject* ret = null;

			//Check is group or object
			std::vector<util::JSON> val = ((const util::JSON)(json["children"])).GetArray<util::JSON>();
			if (val.empty())
				ret = new CObject();
			else
				ret = new CGroup();

			ret->LoadJson(json);

			return ret;
		}

		const PartList GetPartList(const CObject* const pObj, const bool& reverse_priority)
		{
			assert(pObj);
			PartList list;

			std::vector<IPart*> vec = pObj->GetParts();
			for (ulong i = 0; i < vec.size(); ++i)
			{
				list.push_back(vec[i]);
			}

			if (pObj->IsGroup())
			{
				const CGroup* const pGroup = static_cast<const CGroup* const>(pObj);
				TraverseChildren(pGroup, [&list](CObject* pObj)
				{
					std::vector<IPart*> vec = pObj->GetParts();
					for (ulong i = 0; i < vec.size(); ++i)
					{
						list.push_back(vec[i]);
					}
				});
			}

			//Type of sorting function
			std::function<bool(IPart* lhs, IPart* rhs)> func;
			if (reverse_priority)
			{
				func = [](IPart* lhs, IPart* rhs)
				{
					return lhs->GetPriority() > rhs->GetPriority();
				};
			}
			else
			{
				func = [](IPart* lhs, IPart* rhs)
				{
					return lhs->GetPriority() < rhs->GetPriority();
				};
			}

			list.sort(func);

			return list;
		}

		const PartList GetAncestorPartList(const CObject* const pObj, const bool & reverse_priority)
		{
			assert(pObj);
			PartList list;

			CObject* parent = pObj->GetParent();
			while (parent != null)
			{
				std::vector<IPart*> vec = parent->GetParts();
				for (ulong i = 0; i < vec.size(); ++i)
				{
					list.push_back(vec[i]);
				}
				parent = parent->GetParent();
			}

			//Type of sorting function
			std::function<bool(IPart* lhs, IPart* rhs)> func;
			if (reverse_priority)
			{
				func = [](IPart* lhs, IPart* rhs)
				{
					return lhs->GetPriority() > rhs->GetPriority();
				};
			}
			else
			{
				func = [](IPart* lhs, IPart* rhs)
				{
					return lhs->GetPriority() < rhs->GetPriority();
				};
			}

			list.sort(func);

			return list;
		}

		void TraverseAncestors(const CObject* const that
			, std::function<void(CObject*const)> func
			, util::EDirection eDirection)
		{
			assert(that);
			switch (eDirection)
			{
			case util::EDirection::UP:
			{
				CObject* pParent = that->GetParent();
				if (pParent)
				{
					func(pParent);
					TraverseAncestors(pParent, func, eDirection);
				}
				break;
			}
			case util::EDirection::DOWN:
			{
				std::vector<CObject*> parents;
				CObject* pParent = that->GetParent();
				while (pParent)
				{
					parents.push_back(pParent);
					pParent = pParent->GetParent();
				}
				for (int i = (int)parents.size() - 1; i >= 0; --i)
				{
					func(parents[i]);
				}
				break;
			}
			default:
				assert(false);
			}
			
		}

		void TraverseChildren(const CGroup* const that
			, std::function<void(CObject*const)> func
			, int depth)
		{
			assert(that);
			std::vector<CObject*> children = that->GetChildren();
			const size_t size = children.size();
			for (size_t i = 0; i < size; ++i)
			{
				func(children[i]);
			}
			
			if (depth == 0)
				return;
			
			for (size_t i = 0; i < size; ++i)
			{
				if (children[i]->IsGroup())
				{
					CGroup* pGroup = static_cast<CGroup*>(children[i]);
					TraverseChildren(pGroup, func, depth - 1);
				}
			}
		}

		void TraverseChildrenBreak(CGroup* const that
			, std::function<bool(CObject*const)> func
			, const int& depth)
		{
			__not_implemented(TraverseChildrenBreak);
			assert(false);
		}


		/*typedef std::pair<CObject*, bool> Node;
		void TraverseBreadthFirst(CObject* root, std::function<void(CObject*)> func)
		{
			assert(root);

			std::queue<Node> queue;
			queue.push(Node(root,true));
			func(root);

			while (!queue.empty())
			{
				Node node = queue.front();
				queue.pop();
				if (node.first->IsGroup())
				{
					CGroup* pGroup = reinterpret_cast<CGroup*>(node.first);
					std::vector<CObject*> children = pGroup->GetChildren();
					for (int i = 0, size = children.size(); i < size; ++i)
					{
						func(children[i]);
						queue.push(Node(children[i], true));
					}
				}
			}
		}
		void TraverseDepthFirst(CObject* root, std::function<void(CObject*)> func)
		{
			assert(root);

			std::stack<Node> stack;
			stack.push(Node(root, false));
			while (!stack.empty())
			{
				Node node = stack.top();
				stack.pop();
				if (!node.second)
				{
					func(node.first);
					node.second = true;
					if (node.first->IsGroup())
					{
						CGroup* pGroup = reinterpret_cast<CGroup*>(node.first);
						std::vector<CObject*> children = pGroup->GetChildren();
						for (int i = 0, size = children.size(); i < size; ++i)
						{
							stack.push(Node(children[i], false));
						}
					}
				}
			}
		}
*/
	}
}
