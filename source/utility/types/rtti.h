#pragma once

namespace util
{
	static int s_nextTypeId = 0;

	struct TypeInfo
	{
		const TypeInfo* m_pParent;
		const char* m_szClassName;
		const int m_nTypeId;

		bool IsDerivedFrom(const TypeInfo* const that) const;
	};

	//Put this in the class header
#define DECLARE_RTTI_INFO()														\
	public:																		\
		static util::TypeInfo s_typeInfo;								\
		virtual const util::TypeInfo* GetTypeInfo() const

		//Put this in the class cpp - of the child classes
#define DEFINE_RTTI_INFO(CLASS, BASE_CLASS)										\
	util::TypeInfo CLASS::s_typeInfo =									\
		{																		\
			&BASE_CLASS::s_typeInfo,											\
			#CLASS																\
			++util::s_nextTypeId											\
		}																		\
		const util::TypeInfo* CLASS::GetTypeInfo() const					\
		{																		\
			return &CLASS::s_typeInfo;											\
		}

		//Put this in the base class cpp
#define DEFINE_RTTI_INFO_BASE(CLASS)											\
		util::TypeInfo CLASS::s_typeInfo =								\
		{																		\
			nullptr, /* No base class */										\
			#CLASS																\
			++util::s_nextTypeId											\
		};																		\
		const util::TypeInfo* CLASS::GetTypeInfo() const					\
		{																		\
			return &CLASS::s_typeInfo;											\
		}
}