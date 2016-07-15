#pragma once
#include "rapidjson/document.h"
#include "time/util_time.h"
#include "types/color.h"
#include "helper/macro.h"
#include "math/util_math.h"
#include "math/Matrix.h"
#include <vector>

//TODO - change out Matrix for transform

namespace util
{
	//this class extends the functionality of rapidjson::Document for use with our custom types
	class JSON : public rapidjson::Document
	{
	public:
		JSON() : ptr_alloc(null){}
		JSON(const JSON& that) : ptr_alloc(null) { *this = that; }
		JSON(const rapidjson::Document& that) : ptr_alloc(null) { this->CopyFrom(that, GetAllocator()); }
		JSON(const rapidjson::Value& that) : ptr_alloc(null) { this->CopyFrom(that, GetAllocator()); }
		JSON& operator= (const JSON& that)
		{
			if (this == &that) //self assignment check
				return *this;
			if (that.ptr_alloc == null)
				this->CopyFrom(that, GetAllocator());
			else
				this->CopyFrom(that, *(that.ptr_alloc));
			this->ptr_alloc = that.ptr_alloc;
			return *this;
		}

		const std::string Read() const;

		const Time				GetTime() const;
		const Color				GetColor() const;
		const vec2				GetVec2() const;
		const vec3				GetVec3() const;
		const vec4				GetVec4() const;
		const AABB				GetAabb() const;
		const Matrix			GetMatrix() const;
		template<typename type>
		const std::vector<type> GetArray(void) const;

		void SetTime(const Time& val, rapidjson::Document::AllocatorType& allocator);
		void SetColor(const Color& val, rapidjson::Document::AllocatorType& allocator);
		void SetVec2(const vec2& val, rapidjson::Document::AllocatorType& allocator);
		void SetVec3(const vec3& val, rapidjson::Document::AllocatorType& allocator);
		void SetVec4(const vec4& val, rapidjson::Document::AllocatorType& allocator);
		void SetAabb(const AABB& val, rapidjson::Document::AllocatorType& allocator);
		void SetMatrix(const Matrix& val, rapidjson::Document::AllocatorType& allocator);
		template<typename type>
		const void WriteArray(std::vector<type> val, rapidjson::Document::AllocatorType& allocator); //this is called 'write' because 'setarray' is already used by rapidjson::value

		rapidjson::Document::AllocatorType* ptr_alloc;
	};

	template<typename type>
	const void JSON::WriteArray(std::vector<type> val, rapidjson::Document::AllocatorType& allocator)
	{
		SetArray();
		for (size_t i = 0; i < val.size(); ++i)
			PushBack(val[i], allocator);
	}
	template<>
	const void JSON::WriteArray(std::vector<Time> val, rapidjson::Document::AllocatorType& allocator);
	template<>
	const void JSON::WriteArray(std::vector<Color> val, rapidjson::Document::AllocatorType& allocator);
	template<>
	const void JSON::WriteArray(std::vector<vec2> val, rapidjson::Document::AllocatorType& allocator);
	template<>
	const void JSON::WriteArray(std::vector<vec3> val, rapidjson::Document::AllocatorType& allocator);
	template<>
	const void JSON::WriteArray(std::vector<vec4> val, rapidjson::Document::AllocatorType& allocator);
	template<>
	const void JSON::WriteArray(std::vector<Matrix> val, rapidjson::Document::AllocatorType& allocator);

	/*
	EXAMPLES

	//READING
	JSON json;
	json.Parse("{\"name\": \"play\"}");
	bool success = !json.HasParseError();

	//WRITING - allways use the root json instance's allocator
	JSON temp;
	temp.SetObject();
	temp.AddMember("key1", "value1", json.GetAllocator());
	temp.AddMember("key2", "value2", json.GetAllocator());
	json.SetObject();
	json.Set(temp, json.GetAllocator());

	//ARRAY - allways use the root json instance's allocator
	json.SetArray();
	for (size_t i = 0; i < val.size(); ++i)
	json.PushBack(val[i], json.GetAllocator);
	*/
}