#include "color.h"
#include "helper/func.h"
#include <sstream>

namespace util
{
#define _COLOR_DEF(NAME, ARGB) STATIC const Color Color::NAME(ARGB);
#		include "color.def"
#undef _COLOR_DEF

	Color::Color()
		: glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)
	{
	}

	Color::Color(const glm::float32& r, const glm::float32& g, const glm::float32& b, const glm::float32& a)
		: glm::vec4(r, g, b, a)
	{
	}

	Color::Color(const glm::f32vec4& vec)
		: glm::vec4(vec)
	{
	}

	Color::Color(const glm::u32& hex)
	{
		SetHex(hex);
	}

	Color::Color(const std::string& szHex)
	{
		SetHexString(szHex);
	}

	const glm::u8vec4 Color::Get8Bit() const
	{
		glm::u8vec4 ret(
			glm::u8(Clamp(this->r * 255, 0, 255)),
			glm::u8(Clamp(this->g * 255, 0, 255)),
			glm::u8(Clamp(this->b * 255, 0, 255)),
			glm::u8(Clamp(this->a * 255, 0, 255)));
		return ret;
	}
	void Color::Set8Bit(const glm::u8vec4& bit)
	{
		r = bit.r / 255.0f;
		g = bit.g / 255.0f;
		b = bit.b / 255.0f;
		a = bit.a / 255.0f;
	}

	const glm::u32 Color::GetHex() const
	{
		const glm::u8vec4 tmp = Get8Bit();
		glm::u32 ret = *(glm::u32*)&tmp;
		return ret;
	}
	void Color::SetHex(const glm::u32& hex)
	{
		glm::u8vec4 vec = *(glm::u8vec4*)&hex;
		//swap values because of byte order
		std::swap(vec.r, vec.a);
		std::swap(vec.g, vec.b);
		Set8Bit(vec);
	}

	const std::string Color::GetHexString() const
	{
		std::stringstream stream;
		stream << std::hex << GetHex();
		return stream.str();
	}
	void Color::SetHexString(const std::string& szHex)
	{
		glm::u32 fin;
		std::stringstream stream;
		stream << std::hex << szHex.c_str();
		stream >> fin;
		SetHex(fin);
	}

	const glm::float32 Color::GetHue() const
	{
		return GetHSV().x;
	}
	void Color::SetHue(const glm::float32& hue)
	{
		glm::f32vec4 hsv = GetHSV();
		hsv.x = hue;
		SetHSV(hsv);
	}

	const glm::float32 Color::GetSaturation() const
	{
		return GetHSV().y;
	}
	void Color::SetSaturation(const glm::float32& sat)
	{
		glm::f32vec4 hsv = GetHSV();
		hsv.y = sat;
		SetHSV(hsv);
	}

	const glm::float32 Color::GetValue() const
	{
		return GetHSV().z;
	}
	void Color::SetValue(const glm::float32& val)
	{
		glm::f32vec4 hsv = GetHSV();
		hsv.z = val;
		SetHSV(hsv);
	}

	const glm::f32vec4 Color::GetHSV() const
	{
		glm::f32vec4 ret;										// x == hue, y == sat, z == val, w/a == alpha
		ret.a = this->a;

		float fMin = Min(this->r, Min(this->g, this->b));		//Min. value of RGB
		float fMax = Max(this->r, Max(this->g, this->b));		//Max. value of RGB
		float fDelta = fMax - fMin;								//Delta RGB value 

		ret.z = fMax;											//V component

		if (fDelta == 0)										//This is a gray, no chroma...
		{
			ret.x = 0;											//HSV results from 0 to 1
			ret.y = 0;
		}
		else													//Chromatic data...
		{
			ret.y = fDelta / fMax;								//S component

			float del_R = (((fMax - r) / 6) + (fDelta / 2)) / fDelta;
			float del_G = (((fMax - g) / 6) + (fDelta / 2)) / fDelta;
			float del_B = (((fMax - b) / 6) + (fDelta / 2)) / fDelta;


			if (this->r == fMax)
				ret.x = del_B - del_G;
			else if (this->g == fMax)
				ret.x = (1.0f / 3.0f) + del_R - del_B;
			else if (this->b == fMax)
				ret.x = (2.0f / 3.0f) + del_G - del_R;

			if (ret.x < 0)
				ret.x += 1.0f;
			if (ret.x > 1)
				ret.x -= 1.0f;
		}

		return ret;
	}
	void Color::SetHSV(const glm::f32vec4& hsv)
	{
		int i;
		float f, p, q, t;

		this->a = hsv.a; //Alpha

		if (hsv.y == 0)
		{
			// achromatic (grey)
			this->r = this->g = this->b = hsv.z;
			return;
		}

		float h = hsv.x * 360.0f;
		h /= 60;			// sector 0 to 5
		i = int(floor(h));
		f = h - i;			// factorial part of h
		p = hsv.z * (1.0f - hsv.y);
		q = hsv.z * (1.0f - hsv.y * f);
		t = hsv.z * (1.0f - hsv.y * (1.0f - f));
		switch (i)
		{
		case 0:
			this->r = hsv.z;
			this->g = t;
			this->b = p;
			break;
		case 1:
			this->r = q;
			this->g = hsv.z;
			this->b = p;
			break;
		case 2:
			this->r = p;
			this->g = hsv.z;
			this->b = t;
			break;
		case 3:
			this->r = p;
			this->g = q;
			this->b = hsv.z;
			break;
		case 4:
			this->r = t;
			this->g = p;
			this->b = hsv.z;
			break;
		default:
			this->r = hsv.z;
			this->g = p;
			this->b = q;
			break;
		}
	}

	// STATIC
	STATIC const Color Color::Blend(Color one, Color two, float lerp) //Blends two colors, lerp value is between 0.0f and 1.0f, 0.0f == all one clr, 1.0f == all two clr
	{
		//Clamp lerp value
		lerp = Clamp(lerp, 0.0f, 1.0f);

		//We are gonna do proper color blending - YEAH : https://www.youtube.com/watch?v=LKnqECcg6Gw

		//Get the real values by squaring
		one.r = glm::pow(one.r, 2);
		one.g = glm::pow(one.g, 2);
		one.b = glm::pow(one.b, 2);
		one.a = glm::pow(one.a, 2);
		two.r = glm::pow(two.r, 2);
		two.g = glm::pow(two.g, 2);
		two.b = glm::pow(two.b, 2);
		two.a = glm::pow(two.a, 2);

		//Perform the blend
		Color ret = one + (two - one) * lerp;

		//Put the values back to their square root
		ret.r = glm::sqrt(ret.r);
		ret.g = glm::sqrt(ret.g);
		ret.b = glm::sqrt(ret.b);
		ret.a = glm::sqrt(ret.a);

		return ret;
	}
}