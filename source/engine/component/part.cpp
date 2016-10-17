#include "part.h"
#include "object.h"

namespace baka
{
	namespace component
	{
		static const u16 INVALID_PART_INDEX = 0xFFFF;

		STATIC std::map<std::string, PartFunctor>& IPart::GetNameMap()
		{
			static std::map<std::string, PartFunctor> impl;
			return impl;
		}
		STATIC std::map<PartTypeKey, PartFunctor>& IPart::GetKeyMap()
		{
			static std::map<PartTypeKey, PartFunctor> impl;
			return impl;
		}
		STATIC PartPriorityMap IPart::s_vUpdateMap;
		STATIC PartList IPart::s_vPendingPartList;
		STATIC int IPart::s_nextPartTypeId = 0;

		STATIC void IPart::UpdateParts(const util::Time& dt)
		{
			CleanParts();
			for (PartPriorityIter map_iter = s_vUpdateMap.begin(); map_iter != s_vUpdateMap.end(); ++map_iter)
			{
				PartList& list = (*map_iter).second;
				for (PartIter part_iter = list.begin(); part_iter != list.end(); ++part_iter)
				{
					if ((*part_iter)->m_bUpdateEnabled)
						(*part_iter)->Update(dt);
				}
			}
		}
		STATIC void IPart::CleanParts(void)
		{
			for (PartIter iter = s_vPendingPartList.begin(); iter != s_vPendingPartList.end(); ++iter)
			{
				IPart* part = (*iter);
				part->m_bPendingUpdate = false;
				PartList& list = s_vUpdateMap[part->m_priority];
				if (part->m_bUpdateEnabled) //Enable
				{
					// Guard against enabling multiple times
					if (part->m_usPartIndex == INVALID_PART_INDEX)
					{
						part->m_usPartIndex = (u16)list.size();
						list.push_back(part);
					}
				}
				else //Disable
				{
					if (part->m_usPartIndex != INVALID_PART_INDEX)
					{
						if (!list.empty())
						{
							PartIter ours = list.begin();
							std::advance(ours, part->m_usPartIndex);
							PartIter back = list.end();
							std::advance(back, -1);
							std::iter_swap(ours, back);
							(*ours)->m_usPartIndex = part->m_usPartIndex; //NOTE this is no longer 'ours' it is now the original 'back'
							list.pop_back();
						}
						//Invalidate the index
						part->m_usPartIndex = INVALID_PART_INDEX;
					}
				}
			}
			s_vPendingPartList.clear();
		}
		STATIC IPart* IPart::CreatePart(const std::string& type)
		{
			auto find = IPart::GetNameMap().find(type);
			assert(find != IPart::GetNameMap().end() && "This part type doesn't exist!");
			return find->second();
		}
		STATIC IPart* IPart::CreatePart(const int type)
		{
			auto find = IPart::GetKeyMap().find(type);
			assert(find != IPart::GetKeyMap().end());
			return find->second();
		}
		STATIC void IPart::RegisterPart(const std::string& typeName, const PartTypeKey& key, const PartFunctor func)
		{
			std::map<std::string, PartFunctor>& name_map = IPart::GetNameMap();
			std::map<PartTypeKey, PartFunctor>& key_map = IPart::GetKeyMap();

			//Each part should be unique
			assert(name_map.find(typeName) == name_map.end() && "You have a duplicate part!");
			assert(key_map.find(key) == key_map.end() && "You have a duplicate part!");

			//int key = ++IPart::s_nextPartTypeId;
			IPart::GetNameMap()[typeName] = func;
			IPart::GetKeyMap()[key] = func;
		}

		//------------------------------------------------------------------------------

		IPart::IPart(void)
			: m_pOwner(null)
			, m_priority(0)
			, m_bUpdateEnabled(false)
			, m_bPendingUpdate(false)
			, m_usPartIndex(INVALID_PART_INDEX)
			, m_eFlag(EFlag::ACTIVE)
		{
		}
		VIRTUAL IPart::~IPart(void)
		{
			DisableUpdateImmediate();
		}
		IPart::IPart(const IPart& that)
		{
			*this = that;
		}
		IPart& IPart::operator= (const IPart& that)
		{
			//Check for self assignment
			if (this == &that)
				return *this;

			assert(this->GetType() == that.GetType() && "WE SHOULDN'T COPY OR CLONE A PART ONTO A DIFFERENT TYPE OF PART");
			m_pOwner = null;
			return *this;
		}

		VIRTUAL void IPart::Reset(void)
		{
			this->Exit();
			this->Init();
		}

		VIRTUAL void IPart::Destroy(void)
		{
			this->Exit();
			delete this;
		}

		VIRTUAL void IPart::Copy(IPart* const that)
		{
			*this = *that;
		}
		VIRTUAL IPart* IPart::Clone(void)
		{
			IPart* ret = IPart::CreatePart(this->GetType());
			ret->Copy(this);
			return ret;
		}

		void IPart::EnableUpdate(void)
		{
			if (m_bUpdateEnabled)
				return;
			m_bUpdateEnabled = true;

			if (m_bPendingUpdate)
				return;
			m_bPendingUpdate = true;

			s_vPendingPartList.push_back(this);
		}
		void IPart::DisableUpdate(void)
		{
			if (!m_bUpdateEnabled)
				return;
			m_bUpdateEnabled = false;

			if (m_bPendingUpdate)
				return;
			m_bPendingUpdate = true;

			s_vPendingPartList.push_back(this);
		}
		void IPart::DisableUpdateImmediate(void)
		{
			if (this->m_usPartIndex != INVALID_PART_INDEX)
			{
				PartList& list = s_vUpdateMap[this->m_priority];
				if (!list.empty())
				{
					PartIter ours = list.begin();
					std::advance(ours, this->m_usPartIndex);
					PartIter back = list.end();
					std::advance(back, -1);
					std::iter_swap(ours, back); 
					(*ours)->m_usPartIndex = this->m_usPartIndex; //NOTE this is no longer 'ours' it is now the original 'back'
					list.pop_back();
				}
				//Invalidate the index
				this->m_usPartIndex = INVALID_PART_INDEX;
			}
		}
	}
}
